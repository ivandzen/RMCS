#include "qostreamcontroller.h"
#include <qcore/controller/qrwpropertycontroller.h>

QOStreamProcessor::QOStreamProcessor(QOStreamController * controller) :
    QThread(controller),
    _running(false),
    _controller(controller),
    _currentPacket(0)
{
}

void QOStreamProcessor::run()
{
    _running = true;

    //! Pointer to the beginning of stream buffer
    Data * ptr_base = reinterpret_cast<Data*>(_controller->_buffer.data());
    _currentPacket = 0;

    while(true)
    {
        //! Process one packet (distribute packet data across channels)
        _controller->_readyPackets.acquire(1);

        Data * ptr = ptr_base + _currentPacket * _controller->_packetSize;

        _controller->processChannels(ArrayRef<Data>(ptr, _controller->_packetSize));

        ++_currentPacket;
        if(_currentPacket >= _controller->_bufCapacity)
            _currentPacket = 0;

        //! Check exit condition
        {
            QMutexLocker locker(&_mutex);
            if(!_running)
                break;
        }

        _controller->_freePackets.release(1);
    }
}

////////////////////////////////////////////////////////////////////////////////

QOStreamReader::QOStreamReader(uint16_t period,
                               QOStreamController * controller) :
    QThread(controller),
    _running(false),
    _controller(controller),
    _currentPacket(0),
    _period(period)
{

}

void QOStreamReader::endTransfer(Length_t readyPackets,
                                 QOStreamReader::TferResult res)
{
    int last_resources = _controller->_bufCapacity - _controller->_freePackets.available();

    switch(res)
    {
    case TFER_COMPLETE :
    {
        _controller->_readyPackets.release(readyPackets);
        _currentPacket += readyPackets;
        if(_currentPacket >= _controller->_bufCapacity)
            _currentPacket = 0;
        break;
    }

    case TFER_ERROR :
        _controller->_freePackets.release(_controller->_ppt);
        break;

    case TFER_STOP :
        _controller->_freePackets.release(last_resources);
        emit shutDown();
        break;
    }
}

void QOStreamReader::run()
{
    _running = true;
    _currentPacket = 0;
    logMessage("Reader started");
    while(_running)
    {
        Length_t num_packets = _controller->_bufCapacity - _currentPacket;
        if(num_packets > _controller->_ppt)
            num_packets = _controller->_ppt;

        char * ptr = _controller->_buffer.data() + _currentPacket * _controller->_packetSize;

        _controller->_freePackets.acquire(num_packets);
        if(!beginTransfer(ptr, num_packets, _controller->_packetSize))
            _controller->_freePackets.release(num_packets);

        QThread::usleep(_period * 1000);
    }
}

void QOStreamReader::logMessage(const QString &message)
{
    _controller->logMessage(message);
}

////////////////////////////////////////////////////////////////////////////////

QOStreamController::QOStreamController(Length_t buf_cap,
                                       Length_t ppt,
                                       NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * conn) :
    QNodeController (node_id, parent_id, name, conn),
    _enabled(nullptr),
    _packetSize(0),
    _bufCapacity(buf_cap),
    _ppt(ppt),
    _processor(new QOStreamProcessor(this)),
    _reader(nullptr)
{
    //! For simplicity
    Q_ASSERT((buf_cap % ppt) == 0);
    _freePackets.release(buf_cap);
}

bool QOStreamController::addChannel(QOStreamChannelController * channel)
{
    _channels.push_back(channel);
    return true;
}

bool QOStreamController::setBufCapacity(Length_t cap)
{
    if(cap == 0)
    {
        logMessage("Unable to set zero buffer capacity!");
        return false;
    }

    if((_reader != nullptr) && (_reader->isRunning()))
    {
        logMessage("Could not change buffer capacity cause reader is running.");
        return false;
    }

    if(_processor->isRunning())
    {
        logMessage("Could not change buffer capacity cause processor is running.");
        return false;
    }

    _bufCapacity = cap;
    _buffer.resize(_bufCapacity * _packetSize);
    return true;
}

bool QOStreamController::setPPT(Length_t ppt)
{
    if(ppt == 0)
    {
        logMessage("Unable to set zero PPT!");
        return false;
    }

    if((_reader != nullptr) && (_reader->isRunning()))
    {
        logMessage("Could not change PPT cause reader is running.");
        return false;
    }

    if(_processor->isRunning())
    {
        logMessage("Could not change PPT cause processor is running.");
        return false;
    }

    _ppt = ppt;
    return true;
}

void QOStreamController::processChannels(const ArrayRef<Data> & buffer)
{
    Data * ptr_base = buffer.data();
    for(int i = 0; i < _channels.size(); ++i)
    {
        QOStreamChannelController * channel = _channels[i];
        if(channel->dataOffset() + channel->dataLength() <= buffer.size())
            channel->streamDataReceived(ptr_base + channel->dataOffset());
        else
        {
            logMessage("Illegal channel parameters");
            _channels.removeAt(i);
            return;
        }
    }
}

bool QOStreamController::eventInit(DeviceController *device)
{
    _enabled = findChild<QRWPropertyController*>("Enabled", Qt::FindDirectChildrenOnly);
    if(_enabled == nullptr)
        return false;

    connect(_enabled, SIGNAL(valueChanged(QVariant)),
            this, SLOT(eventStreamToggled(QVariant)));

    return QNodeController::eventInit(device);
}

bool QOStreamController::eventData(const ControlPacket & packet)
{
    (void)packet;
    return true;
}

void QOStreamController::eventDestroy()
{
    _processor->stop();
    _reader->stop();
    _processor->deleteLater();
    _reader->deleteLater();
    QNodeController::eventDestroy();
}

bool QOStreamController::setPacketSize(Length_t pack_size)
{
    if((_reader != nullptr) && (_reader->isRunning()))
    {
        logMessage("Could not change buffer size cause reader is running.");
        return false;
    }

    if(_processor->isRunning())
    {
        logMessage("Could not change buffer size cause processor is running.");
        return false;
    }

    if(pack_size == 0) {
        logMessage("Unable to set zero length packet!");
        return false;
    }

    _packetSize = pack_size;
    _buffer.resize(_bufCapacity * _packetSize);
    return true;
}

bool QOStreamController::setReader(QOStreamReader *reader)
{
    if(reader == nullptr) return false;
    _reader = reader;
    connect(_reader, SIGNAL(shutDown()), this, SLOT(onReaderShutDown()));
    return true;
}

void QOStreamController::eventStreamToggled(const QVariant &enabled)
{
    if(!enabled.isValid())
    {
        logMessage("eventStreamToggled : Invalid variant in paramenter");
        return;
    }

    if(_reader == nullptr)
    {
        logMessage("Reader not created. Controller not ready");
        return;
    }

    if(enabled.toBool())
    {
        _processor->start();
        _reader->start();
        logMessage("Turn on UsbOStream");
    }
    else
    {
        _reader->stop();
        _processor->stop();
        logMessage("Turn off UsbOStream");
    }
}

void QOStreamController::onReaderShutDown()
{
    logMessage("Shut down stream");
    _reader->stop();
    _processor->stop();
}
