#include "qusbostreamcontroller.h"
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>

QUsbOStreamProcessor::QUsbOStreamProcessor(QUsbOStreamController * controller) :
    QThread(nullptr),
    _controller(controller)
{

}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamProcessor::run()
{
    _running = true;

    //! Pointer to the beginning of stream buffer
    Data * ptr_base = reinterpret_cast<Data*>(_controller->_buffer.data());

    while(true)
    {
        //! Process one packet (distribute packet data across channels)
        _controller->_usedPackets.acquire(1);

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

void ostreamTransferCallback(libusb_transfer * transfer)
{
    reinterpret_cast<QUsbOStreamController*>(transfer->user_data)->transferCplt(transfer);
}

////////////////////////////////////////////////////////////////////////////////

QUsbOStreamController::QUsbOStreamController(libusb_device_handle * usb_handle,
                                             QDeviceConnection * conn,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name) :
    QOStreamController (node_id, parent_id, name, conn),
    _processor(nullptr),
    _handle(usb_handle),
    _currentPacket(0)
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
    _processor->deleteLater();
    QOStreamController::eventDestroy();
}

////////////////////////////////////////////////////////////////////////////////

bool QUsbOStreamController::eventInit(DeviceController * controller)
{
    if(!QOStreamController::eventInit(controller))
        return false;
    _processor = new QUsbOStreamProcessor(this);
    _processor->moveToThread(thread());
    return true;
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
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamController::eventStreamToggled(bool enabled)
{
    if(enabled)
    {
        beginTransfer();
        _processor->start();
    }
    else
        _processor->stop();
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamController::beginTransfer()
{
    if(!isStreamEnabled())
        return;

    _freePackets.acquire(PACKETS_PER_TRANSFER);

    //! Prepare and submit isochronous usb transfer

    libusb_transfer * tfer = libusb_alloc_transfer(PACKETS_PER_TRANSFER);
    char * ptr = _buffer.data() + _currentPacket * _packetSize;

    libusb_fill_iso_transfer(tfer, _handle, _epAddr,
                             reinterpret_cast<unsigned char*>(ptr),
                             PACKETS_PER_TRANSFER * _packetSize,
                             PACKETS_PER_TRANSFER,
                             ostreamTransferCallback,
                             this, timeout());

    libusb_set_iso_packet_lengths(tfer, _packetSize);

    if(libusb_submit_transfer(tfer) != 0)
        logMessage("Unable to submit isochronous usb transfer");
}

////////////////////////////////////////////////////////////////////////////////

void QUsbOStreamController::transferCplt(libusb_transfer *tfer)
{
    _currentPacket += tfer->num_iso_packets;
    if(_currentPacket + PACKETS_PER_TRANSFER >= NUM_PACKETS)
        _currentPacket = 0;
    _usedPackets.release(tfer->num_iso_packets);
    switch(tfer->status)
    {
    case LIBUSB_TRANSFER_COMPLETED :
    case LIBUSB_TRANSFER_ERROR :
    case LIBUSB_TRANSFER_STALL :
    case LIBUSB_TRANSFER_OVERFLOW :
    case LIBUSB_TRANSFER_TIMED_OUT :
    {
        beginTransfer();
        break;
    }

    case LIBUSB_TRANSFER_NO_DEVICE :
    case LIBUSB_TRANSFER_CANCELLED :
    {
        _processor->stop();
        logMessage("Usb istream stopped.");
        break;
    }
    }
}
