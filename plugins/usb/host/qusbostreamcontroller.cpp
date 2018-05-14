#include "qusbostreamcontroller.h"
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>
#include <QElapsedTimer>

QUsbOStreamProcessor::QUsbOStreamProcessor(QUsbOStreamController * controller) :
    QThread(nullptr),
    _controller(controller),
    _currentPacket(0)
{
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamProcessor::run()
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
        if(_currentPacket >= QUsbOStreamController::NUM_PACKETS)
            _currentPacket = 0;

        _controller->_freePackets.release(1);

        //! Check exit condition
        {
            QMutexLocker locker(&_mutex);
            if(!_running)
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void CALLBACK_ATTRIB ostreamTransferCallback(libusb_transfer * transfer)
{
    reinterpret_cast<QUSBOStreamReader*>(transfer->user_data)->transferCplt(transfer);
}

QUSBOStreamReader::QUSBOStreamReader(UsbEPDescriptor::EPType_t type, QUsbOStreamController * controller) :
    QThread(nullptr),
    _running(false),
    _controller(controller),
    _currentPacket(0),
    _transfer(nullptr),
    _period(1),
    _type(type)
{
}

void QUSBOStreamReader::run()
{
    _running = true;
    _currentPacket = 0;
    logMessage("Reader started");
    while(_running)
    {
        beginTransfer();
        QThread::usleep(_period * 1000);
    }
}

void QUSBOStreamReader::beginTransfer()
{
    int num_packets = QUsbOStreamController::NUM_PACKETS - _currentPacket;
    if(num_packets > QUsbOStreamController::PACKETS_PER_TRANSFER)
        num_packets = QUsbOStreamController::PACKETS_PER_TRANSFER;

    _controller->_freePackets.acquire(num_packets);

    if(_transfer != nullptr)
    {
        logMessage("beginTransfer : not null transfer.");
        return;
    }

    char * ptr = _controller->_buffer.data() + _currentPacket * _controller->_packetSize;


    if(num_packets == 0)
        return;

    switch(_type)
    {
    case UsbEPDescriptor::EP_BULK :
    {
        _transfer = libusb_alloc_transfer(0);
        if(_transfer == nullptr)
        {
            logMessage("beginTransfer : transfer not allocated!");
            return;
        }

        libusb_fill_bulk_transfer(_transfer,
                                  _controller->_handle,
                                  _controller->_epAddr,
                                  reinterpret_cast<unsigned char*>(ptr),
                                  num_packets * _controller->_packetSize,
                                  ostreamTransferCallback, this, 0);
        break;
    }

    case UsbEPDescriptor::EP_ISOC :
    {
        _transfer = libusb_alloc_transfer(num_packets);
        if(_transfer == nullptr)
        {
            logMessage("beginTransfer : transfer not allocated!");
            return;
        }

        libusb_fill_iso_transfer(_transfer,
                                 _controller->_handle,
                                 _controller->_epAddr,
                                 reinterpret_cast<unsigned char*>(ptr),
                                 num_packets * _controller->_packetSize,
                                 num_packets,
                                 ostreamTransferCallback, this, 0);

        libusb_set_iso_packet_lengths(_transfer, _controller->_packetSize);
        break;
    }

    case UsbEPDescriptor::EP_CTRL :
    case UsbEPDescriptor::EP_INTR :
        logMessage("Stream supported only on Bulk and Isochronous endpoints");
        return;
    }

    if(libusb_submit_transfer(_transfer) != 0)
        logMessage("Unable to submit usb transfer");
}

void QUSBOStreamReader::transferCplt(libusb_transfer * tfer)
{
    int ready_packets = 0;
    int last_resources = QUsbOStreamController::NUM_PACKETS - _controller->_freePackets.available();

    switch(_type)
    {
    case UsbEPDescriptor::EP_BULK :
        ready_packets = _transfer->actual_length / _controller->_packetSize;
        break;

    case UsbEPDescriptor::EP_ISOC :
        ready_packets = _transfer->num_iso_packets;
        break;

    case UsbEPDescriptor::EP_CTRL :
    case UsbEPDescriptor::EP_INTR :
        logMessage("Stream supported only on Bulk and Isochronous endpoints");
        return;
    }

    libusb_transfer_status status = tfer->status;
    libusb_free_transfer(tfer);
    _transfer = nullptr;

    switch(status)
    {
    case LIBUSB_TRANSFER_COMPLETED :
    {
        if(ready_packets == 0)
        {
            logMessage("0 packets ready");
            return;
        }

        _controller->_readyPackets.release(ready_packets);
        _currentPacket += ready_packets;
        if(_currentPacket >= QUsbOStreamController::NUM_PACKETS)
            _currentPacket = 0;
        break;
    }

    case LIBUSB_TRANSFER_ERROR :
    {
        logMessage("Transfer error.");
        _controller->_freePackets.release(last_resources);
        emit shutDown();
        break;
    }

    case LIBUSB_TRANSFER_STALL :
    {
        logMessage("Transfer stall.");
        _controller->_freePackets.release(last_resources);
        emit shutDown();
        break;
    }

    case LIBUSB_TRANSFER_OVERFLOW :
    {
        logMessage("Transfer overflow");
        _controller->_freePackets.release(QUsbOStreamController::PACKETS_PER_TRANSFER);
        break;
    }

    case LIBUSB_TRANSFER_TIMED_OUT :
    {
        logMessage("Transfer timeout");
        _controller->_freePackets.release(QUsbOStreamController::PACKETS_PER_TRANSFER);
        break;
    }

    case LIBUSB_TRANSFER_NO_DEVICE :
    {
        logMessage("No device. Stream stopped.");
        _controller->_freePackets.release(last_resources);
        emit shutDown();
        break;
    }

    case LIBUSB_TRANSFER_CANCELLED :
    {
        logMessage("Transfer cancelled.");
        _controller->_freePackets.release(last_resources);
        break;
    }
    }
}

void QUSBOStreamReader::logMessage(const QString & message)
{
    _controller->logMessage(message);
}

////////////////////////////////////////////////////////////////////////////////

QUsbOStreamController::QUsbOStreamController(libusb_device_handle * usb_handle,
                                             QDeviceConnection * conn,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name) :
    QOStreamController (node_id, parent_id, name, conn),
    _processor(nullptr),
    _reader(nullptr),
    _handle(usb_handle),
    _epAddr(0x00),
    _packetSize(0)
{
    //! For simplicity
    Q_ASSERT((NUM_PACKETS % PACKETS_PER_TRANSFER) == 0);
    _freePackets.release(NUM_PACKETS);
}

////////////////////////////////////////////////////////////////////////////////

QNodeController *QUsbOStreamController::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection *conn)
{
    return new QUsbOStreamController(_handle, conn, node_id, parent_id, name);
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamController::eventDestroy()
{
    _processor->stop();
    _reader->stop();
    _processor->deleteLater();
    _reader->deleteLater();
    QOStreamController::eventDestroy();
}

////////////////////////////////////////////////////////////////////////////////

bool QUsbOStreamController::eventInit(DeviceController * controller)
{
    _processor = new QUsbOStreamProcessor(this);
    _processor->moveToThread(thread());
    return QOStreamController::eventInit(controller);
}

////////////////////////////////////////////////////////////////////////////////

bool QUsbOStreamController::eventSetup(const ControlPacket & packet)
{
    UsbStreamSettings settings(packet);
    if(!settings.isValid())
        return false;

    _epAddr = settings.epAddr();
    _packetSize = settings.packetSize();
    _buffer.resize(_packetSize * NUM_PACKETS);

    libusb_config_descriptor * config;

    int result = libusb_get_active_config_descriptor(libusb_get_device(_handle), &config);
    if(result != LIBUSB_SUCCESS)
    {
        logMessage("Unable to get config descriptor");
        logMessage(libusb_error_name(result));
        return false;
    }

    const struct libusb_interface_descriptor * iface = config->interface[ISOUSB_BASE_INTERFACE].altsetting;
    for(uint8_t ep_num = 0; ep_num < iface->bNumEndpoints; ++ep_num)
        if(iface->endpoint[ep_num].bEndpointAddress == _epAddr)
        {
            _reader = new QUSBOStreamReader(UsbEPDescriptor::EPType_t(iface->endpoint[ep_num].bmAttributes & 0x03), this);
            _reader->moveToThread(thread());
            connect(_reader, SIGNAL(shutDown()), this, SLOT(onReaderShutDown()));
            logMessage("QUsbOStreamReader created");
            libusb_free_config_descriptor(config);
            return true;
        }

    logMessage("Enpoint not found");
    libusb_free_config_descriptor(config);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamController::eventStreamToggled(const QVariant & enabled)
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

void QUsbOStreamController::onReaderShutDown()
{
    logMessage("Shut down stream");
    _reader->stop();
    _processor->stop();
}
