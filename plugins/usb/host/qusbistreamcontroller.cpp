#include "qusbistreamcontroller.h"
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>

QUsbIStreamProcessor::QUsbIStreamProcessor(QUsbIStreamController * controller) :
    QThread(nullptr),
    _controller(controller),
    _running(false)
{
}

////////////////////////////////////////////////////////////////////////////////

void QUsbIStreamProcessor::run()
{
    _running = true;
    while(true)
    {
        _controller->acquireUsedPacket();
        _controller->processChannels(ArrayRef<Data>(reinterpret_cast<Data*>(_controller->_buffer.data()),
                                                    Length_t(_controller->_buffer.size())));

        _controller->releaseFreePacket();

        {
            QMutexLocker locker2(&_mutex);
            if(!_running)
                break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void CALLBACK_ATTRIB istreamTransferCallback(libusb_transfer * transfer)
{
    reinterpret_cast<QUsbIStreamController*>(transfer->user_data)
            ->transferCplt(transfer);
}

////////////////////////////////////////////////////////////////////////////////

QUsbIStreamController::QUsbIStreamController(libusb_device_handle * usb_handle,
                                             QDeviceConnection * dev,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name) :
    QIStreamController (node_id, parent_id, name, dev),
    _processor(nullptr),
    _handle(usb_handle)
{
    _freePacket.release(1);
}

////////////////////////////////////////////////////////////////////////////////

QNodeController *QUsbIStreamController::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * conn)
{
    return new QUsbIStreamController(_handle, conn, node_id, parent_id, name);
}

////////////////////////////////////////////////////////////////////////////////

void QUsbIStreamController::eventDestroy()
{
    _processor->deleteLater();
    QIStreamController::eventDestroy();
}

////////////////////////////////////////////////////////////////////////////////

bool QUsbIStreamController::eventInit(DeviceController * controller)
{
    _processor = new QUsbIStreamProcessor(this);
    _processor->moveToThread(thread());
    return QIStreamController::eventInit(controller);
}

////////////////////////////////////////////////////////////////////////////////

bool QUsbIStreamController::eventSetup(const ControlPacket & packet)
{
    UsbStreamSettings settings(packet);
    if(!settings.isValid())
        return false;
    _epAddr = settings.epAddr();
    _buffer.resize(settings.packetSize());
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QUsbIStreamController::eventStreamToggled(bool enabled)
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

void QUsbIStreamController::beginTransfer()
{
    if(!isStreamEnabled())
        return;

    _freePacket.acquire(1);

    libusb_transfer * tfer = libusb_alloc_transfer(1);

    libusb_fill_iso_transfer(tfer, _handle, _epAddr,
                             reinterpret_cast<unsigned char*>(_buffer.data()),
                             _buffer.length(),
                             1, istreamTransferCallback,
                             this, timeout());
    libusb_set_iso_packet_lengths(tfer, _buffer.size());

    int result = libusb_submit_transfer(tfer);
    if(result != 0)
    {
        logMessage("Unable to submit isochronous usb transfer");
        qDebug() << libusb_error_name(result);
    }
}

////////////////////////////////////////////////////////////////////////////////

void QUsbIStreamController::transferCplt(libusb_transfer * tfer)
{
    switch(tfer->status)
    {
    case LIBUSB_TRANSFER_COMPLETED :
    {
        _usedPacket.release(1);
        beginTransfer();
        break;
    }

    case LIBUSB_TRANSFER_ERROR :
    {
        _processor->stop();
        logMessage("Transfer Error. Usb istream stopped.");
        break;
    }

    case LIBUSB_TRANSFER_STALL :
    {
        _processor->stop();
        logMessage("Transfer stall. Usb istream stopped.");
        break;
    }

    case LIBUSB_TRANSFER_OVERFLOW :
    case LIBUSB_TRANSFER_TIMED_OUT :
    {
        _freePacket.release(1);
        beginTransfer();
        break;
    }

    case LIBUSB_TRANSFER_NO_DEVICE :
    {
        _processor->stop();
        logMessage("No device. Usb istream stopped.");
        break;
    }

    case LIBUSB_TRANSFER_CANCELLED :
    {
        _processor->stop();
        logMessage("Transfer cancelled. Usb istream stopped.");
        break;
    }
    }
}
