#include "qusbconnection.h"
#include <core/common/control_protocol.h>
#include <qcore/controller/qdevicecontroller.h>
#include <usb/common/common.h>
#include <usb/host/qusbistreamcontroller.h>
#include <usb/host/qusbostreamcontroller.h>

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

////////////////////////////////////////////////////////////////////////////////

void CALLBACK_ATTRIB controlTransferCallback(struct libusb_transfer * transfer)
{
    QUsbConnection * connection =
            reinterpret_cast<QUsbConnection*>(transfer->user_data);

    connection->transferCallback(transfer);
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

void QUsbConnection::abortCtlTransfer()
{
    _semaphore.release(1);
}

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
#include <QDebug>

void QUsbConnection::logMessage(const QString & message)
{
    qDebug() << message;
}

////////////////////////////////////////////////////////////////////////////////

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
            rxHandler(transfer)(_rx_pack);
            break;

        case LIBUSB_TRANSFER_ERROR :
            logMessage("libusb : transfer error");
            errorEvent();
            break;

        case LIBUSB_TRANSFER_CANCELLED :
            logMessage("libusb : transfer cancelled");
            errorEvent();
            break;

        case LIBUSB_TRANSFER_STALL :
            logMessage("libusb : transfer stall");
            errorEvent();
            break;

        case LIBUSB_TRANSFER_OVERFLOW :
            logMessage("libusb : transfer overflow");
            errorEvent();
            break;

        case LIBUSB_TRANSFER_NO_DEVICE :
            disconnectEvent();
            break;

        case LIBUSB_TRANSFER_TIMED_OUT :
            timeoutEvent();
            break;
        }

        removeRxHandler(transfer);
    }
    else
    {
        switch(transfer->status)
        {
        case LIBUSB_TRANSFER_COMPLETED :
            txHandler(transfer)();
            break;

        case LIBUSB_TRANSFER_ERROR :
        case LIBUSB_TRANSFER_CANCELLED :
        case LIBUSB_TRANSFER_STALL :
        case LIBUSB_TRANSFER_OVERFLOW :
            errorEvent();
            break;

        case LIBUSB_TRANSFER_NO_DEVICE :
            disconnectEvent();
            break;

        case LIBUSB_TRANSFER_TIMED_OUT :
            timeoutEvent();
            break;
        }

        removeTxHandler(transfer);
    }

    //! Buffer is binded to class object.
    //! It must not being deallocated dynamically
    transfer->buffer = NULL;
    libusb_free_transfer(transfer);
}
