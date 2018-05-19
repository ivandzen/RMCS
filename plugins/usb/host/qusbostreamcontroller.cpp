#include "qusbostreamcontroller.h"
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>
#include <QElapsedTimer>


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
