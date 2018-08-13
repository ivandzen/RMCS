#include <plugins/usb/host/usbbackend.h>
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

QUsbConnection::QUsbConnection(libusb_device_handle * handle,
                               QObject * parent) :
    QDeviceConnection (parent),
    _semaphore(1),
    _handle(handle),
    _rx_pack(_rx_buf + sizeof(libusb_control_setup), MAX_SETUP_DATA_LENGTH),
    _tx_pack(_tx_buf + sizeof(libusb_control_setup), MAX_SETUP_DATA_LENGTH),
    _timeout(1000)
{
}

void CALLBACK_ATTRIB controlTransferCallback(struct libusb_transfer * transfer)
{
    QUsbConnection * connection =
            reinterpret_cast<QUsbConnection*>(transfer->user_data);

    connection->transferCallback(transfer);
}

ControlPacket QUsbConnection::beginCtlTransfer(Length_t max_length)
{
    _semaphore.acquire(1);

    if(max_length > (MAX_SETUP_DATA_LENGTH - sizeof(libusb_control_setup)))
    {
        logMessage(QString("Warning : usb connection has only %1 byte(s) "
                           "of TX-buffer but %2 byte(s) were anounced.")
                   .arg(MAX_SETUP_DATA_LENGTH)
                   .arg(max_length));
    }
    return _tx_pack;
}

bool QUsbConnection::endCtlTransfer(NodeID_t node_id,
                                    CtlReq_t type,
                                    const ControlPacket & packet,
                                    const TransmitHandler & handler)
{
    libusb_transfer * tfer = libusb_alloc_transfer(0);
    libusb_fill_control_setup(_tx_buf,
                              LIBUSB_RECIPIENT_INTERFACE |
                              LIBUSB_ENDPOINT_OUT |
                              LIBUSB_REQUEST_TYPE_CLASS,
                              type,
                              node_id,
                              ISOUSB_BASE_INTERFACE,
                              sizeof(ControlPacket::Header) +
                              packet.payloadLength());

    libusb_fill_control_transfer(tfer, _handle, _tx_buf,
                                 controlTransferCallback,
                                 this, _timeout);

    int res = libusb_submit_transfer(tfer);
    switch(res)
    {
    case 0 :
    {
        _tx_transfers.insert(tfer, handler);
        return true;
    }

    case LIBUSB_ERROR_NO_DEVICE :
        logMessage("libusb : no device");
        break;
    case LIBUSB_ERROR_BUSY :
        logMessage("libusb : busy");
        break;
    case LIBUSB_ERROR_NOT_SUPPORTED :
        logMessage("libusb : transfer not supported");
        break;
    default :
        logMessage(libusb_error_name(res));
        break;
    }

    return false;
}

void QUsbConnection::abortCtlTransfer()
{
    _semaphore.release(1);
}

bool QUsbConnection::requestCtlPacket(NodeID_t node_id,
                                      CtlReq_t type,
                                      Length_t max_length,
                                      const ReceiveHandler & handler)
{
    _semaphore.acquire(1);

    libusb_transfer * tfer = libusb_alloc_transfer(0);
    libusb_fill_control_setup(_rx_buf,
                              LIBUSB_RECIPIENT_INTERFACE |
                              LIBUSB_ENDPOINT_IN |
                              LIBUSB_REQUEST_TYPE_CLASS,
                              type,
                              node_id,
                              ISOUSB_BASE_INTERFACE,
                              max_length - sizeof(libusb_control_setup));

    libusb_fill_control_transfer(tfer, _handle, _rx_buf,
                                 controlTransferCallback,
                                 this, _timeout);

    int res = libusb_submit_transfer(tfer);
    switch(res)
    {
    case 0 :
    {
        _rx_transfers.insert(tfer, handler);
        return true;
    }

    case LIBUSB_ERROR_NO_DEVICE :
        logMessage("libusb : no device");
        break;
    case LIBUSB_ERROR_BUSY :
        logMessage("libusb : busy");
        break;
    case LIBUSB_ERROR_NOT_SUPPORTED :
        logMessage("libusb : transfer not supported");
        break;
    default :
        logMessage(libusb_error_name(res));
        break;
    }

    return false;
}

QNodeController *QUsbConnection::createNode(NodeType_t node_type,
                                            NodeID_t node_id,
                                            NodeID_t parent_id,
                                            const QString & name)
{
    switch(node_type)
    {
    case NODE_TYPE_USBISTREAM :
        return new QUsbIStreamController(_handle, this, node_id, parent_id, name);

    case NODE_TYPE_USBOSTREAM :
        return new QUsbOStreamController(_handle, this, node_id, parent_id, name);

    default : break;
    }
    logMessage("Unknown node type. Custom node not created");
    return nullptr;
}

void QUsbConnection::logMessage(const QString & message)
{
    qDebug() << message;
}

#define RECIPIENT_MASK 0x1F
#define REQUEST_TYPE 0x60
#define DIRECTION_MASK 0x80

void QUsbConnection::transferCallback(libusb_transfer * transfer)
{
    _semaphore.release(1);

    libusb_control_setup * ctl_setup = libusb_control_transfer_get_setup(transfer);
    if(((ctl_setup->bmRequestType & RECIPIENT_MASK) != LIBUSB_RECIPIENT_INTERFACE) ||
       ((ctl_setup->bmRequestType & REQUEST_TYPE) != LIBUSB_REQUEST_TYPE_CLASS))
        return;

    if((ctl_setup->bmRequestType & DIRECTION_MASK) == LIBUSB_ENDPOINT_IN)
    {
        switch(transfer->status)
        {
        case LIBUSB_TRANSFER_COMPLETED :
            rxHandler(transfer)(_rx_pack, true);
            break;

        case LIBUSB_TRANSFER_ERROR :
            logMessage("libusb : control transfer error");
            rxHandler(transfer)(_rx_pack, false);
            break;

        case LIBUSB_TRANSFER_CANCELLED :
            logMessage("libusb : control transfer cancelled");
            rxHandler(transfer)(_rx_pack, false);
            break;

        case LIBUSB_TRANSFER_STALL :
            logMessage("libusb : control transfer stall");
            rxHandler(transfer)(_rx_pack, false);
            break;

        case LIBUSB_TRANSFER_OVERFLOW :
            logMessage("libusb : control transfer overflow");
            rxHandler(transfer)(_rx_pack, false);
            break;

        case LIBUSB_TRANSFER_NO_DEVICE :
            logMessage("libusb : no device");
            rxHandler(transfer)(_rx_pack, false);
            break;

        case LIBUSB_TRANSFER_TIMED_OUT :
            logMessage("libusb : timed out");
            rxHandler(transfer)(_rx_pack, false);
            break;
        }

        removeRxHandler(transfer);
    }
    else
    {
        switch(transfer->status)
        {
        case LIBUSB_TRANSFER_COMPLETED :
            txHandler(transfer)(true);
            break;

        case LIBUSB_TRANSFER_ERROR :
            logMessage("libusb : control transfer error");
            txHandler(transfer)(false);
            break;

        case LIBUSB_TRANSFER_CANCELLED :
            logMessage("libusb : control transfer cancelled");
            txHandler(transfer)(false);
            break;

        case LIBUSB_TRANSFER_STALL :
            logMessage("libusb : control transfer stall");
            txHandler(transfer)(false);
            break;

        case LIBUSB_TRANSFER_OVERFLOW :
            logMessage("libusb : control transfer overflow");
            txHandler(transfer)(false);
            break;

        case LIBUSB_TRANSFER_NO_DEVICE :
            logMessage("libusb : no device");
            txHandler(transfer)(false);
            break;

        case LIBUSB_TRANSFER_TIMED_OUT :
            logMessage("libusb : timed out");
            txHandler(transfer)(false);
            break;
        }

        removeTxHandler(transfer);
    }

    //! Buffer is binded to class object.
    //! It must not being deallocated dynamically
    transfer->buffer = NULL;
    libusb_free_transfer(transfer);
}

////////////////////////////////////////////////////////////////////////////////

void CALLBACK_ATTRIB ostreamTransferCallback(libusb_transfer * transfer)
{
    reinterpret_cast<QUSBOStreamReader*>(transfer->user_data)->transferCplt(transfer);
}

QUSBOStreamReader::QUSBOStreamReader(UsbEPDescriptor::EPType_t type,
                                     libusb_device_handle * handle,
                                     uint8_t ep_addr,
                                     QUsbOStreamController * controller) :
    QOStreamReader(1, controller),
    _transfer(nullptr),
    _type(type),
    _handle(handle),
    _epAddr(ep_addr)
{
}

bool QUSBOStreamReader::beginTransfer(char * ptr,
                                      Length_t num_packets,
                                      Length_t pack_size)
{
    if(_transfer != nullptr)
    {
        //logMessage("beginTransfer : not null transfer.");
        return false;
    }

    if(num_packets == 0)
        return false;

    switch(_type)
    {
    case UsbEPDescriptor::EP_BULK :
    {
        _transfer = libusb_alloc_transfer(0);
        if(_transfer == nullptr)
        {
            logMessage("beginTransfer : transfer not allocated!");
            return false;
        }

        libusb_fill_bulk_transfer(_transfer, _handle, _epAddr,
                                  reinterpret_cast<unsigned char*>(ptr),
                                  num_packets * pack_size,
                                  ostreamTransferCallback, this, 1);
        break;
    }

    case UsbEPDescriptor::EP_ISOC :
    {
        _transfer = libusb_alloc_transfer(num_packets);
        if(_transfer == nullptr)
        {
            logMessage("beginTransfer : transfer not allocated!");
            return false;
        }

        libusb_fill_iso_transfer(_transfer, _handle, _epAddr,
                                 reinterpret_cast<unsigned char*>(ptr),
                                 num_packets * pack_size,
                                 num_packets,
                                 ostreamTransferCallback, this, 1);

        libusb_set_iso_packet_lengths(_transfer, pack_size);
        break;
    }

    case UsbEPDescriptor::EP_CTRL :
    case UsbEPDescriptor::EP_INTR :
        logMessage("Stream supported only on Bulk and Isochronous endpoints");
        return false;
    }

    int result = libusb_submit_transfer(_transfer);
    if(result == 0)
        return true;

    logMessage(libusb_error_name(result));
    return  false;
}

void QUSBOStreamReader::transferCplt(libusb_transfer * tfer)
{
    Length_t ready_packets = 0;

    switch(_type)
    {
    case UsbEPDescriptor::EP_BULK :
        ready_packets = _transfer->actual_length / packetSize();
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
        endTransfer(ready_packets, TFER_COMPLETE);
        break;
    }

    case LIBUSB_TRANSFER_ERROR :
    {
        logMessage("Transfer error.");
        endTransfer(ready_packets, TFER_ERROR);
        break;
    }

    case LIBUSB_TRANSFER_STALL :
    {
        logMessage("Transfer stall.");
        endTransfer(ready_packets, TFER_STOP);
        break;
    }

    case LIBUSB_TRANSFER_OVERFLOW :
    {
        logMessage("Transfer overflow");
        endTransfer(ready_packets, TFER_ERROR);
        break;
    }

    case LIBUSB_TRANSFER_TIMED_OUT :
    {
        logMessage("Transfer timeout");
        endTransfer(ready_packets, TFER_ERROR);
        break;
    }

    case LIBUSB_TRANSFER_NO_DEVICE :
    {
        logMessage("No device. Stream stopped.");
        endTransfer(ready_packets, TFER_STOP);
        break;
    }

    case LIBUSB_TRANSFER_CANCELLED :
    {
        logMessage("Transfer cancelled.");
        endTransfer(ready_packets, TFER_STOP);
        break;
    }
    }
}

////////////////////////////////////////////////////////////////////////////////

#define USB_DEF_BUF_CAP 128
#define USB_DEF_PPT     2

QUsbOStreamController::QUsbOStreamController(libusb_device_handle * usb_handle,
                                             QDeviceConnection * conn,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name) :
    QOStreamController (USB_DEF_BUF_CAP,
                        USB_DEF_PPT, node_id,
                        parent_id, name, conn),
    _handle(usb_handle)
{
}

QNodeController *QUsbOStreamController::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection *conn)
{
    return new QUsbOStreamController(_handle, conn, node_id, parent_id, name);
}

bool QUsbOStreamController::eventSetup(const ControlPacket & packet)
{
    UsbStreamSettings settings(packet);
    if(!settings.isValid())
        return false;

    setPacketSize(settings.packetSize());

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
        if(iface->endpoint[ep_num].bEndpointAddress == settings.epAddr())
        {
            UsbEPDescriptor::EPType_t ep_type = UsbEPDescriptor::EPType_t (iface->endpoint[ep_num].bmAttributes & 0x03);
            bool result = setReader(new QUSBOStreamReader(ep_type, _handle, settings.epAddr(), this));
            if(!result)
                logMessage("QUsbOStreamReader not created!");

            libusb_free_config_descriptor(config);
            return result;
        }

    logMessage("Enpoint not found");
    libusb_free_config_descriptor(config);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

QUsbIStreamProcessor::QUsbIStreamProcessor(QUsbIStreamController * controller) :
    QThread(nullptr),
    _controller(controller),
    _running(false)
{
}

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

QNodeController *QUsbIStreamController::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * conn)
{
    return new QUsbIStreamController(_handle, conn, node_id, parent_id, name);
}

void QUsbIStreamController::eventDestroy()
{
    _processor->deleteLater();
    QIStreamController::eventDestroy();
}

bool QUsbIStreamController::eventInit(DeviceController * controller)
{
    _processor = new QUsbIStreamProcessor(this);
    _processor->moveToThread(thread());
    return QIStreamController::eventInit(controller);
}

bool QUsbIStreamController::eventSetup(const ControlPacket & packet)
{
    UsbStreamSettings settings(packet);
    if(!settings.isValid())
        return false;
    _epAddr = settings.epAddr();
    _buffer.resize(settings.packetSize());
    return true;
}

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


