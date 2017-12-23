/*
 * RMCSUsbInterface.cpp
 *
 *  Created on: Dec 16, 2017
 *      Author: ivan
 */

#include <plugins/usb/device/RMCSUsbIface.h>
#include <plugins/usb/common/common.h>
#include <core/device/istreamnode.h>
#include <core/device/ostreamnode.h>

#define RMCS_USB_DEVICE_CLASS 0x45

///////////////////////////////////////////////////////////////////////////////////////

RMCSUsbIface::RMCSUsbIface(const UsbInterfaceDescriptor & desc) :
	Device(64),
	XUsbIface(desc),
	_setupRequest(nullptr)
{
}

RMCSUsbIface::~RMCSUsbIface()
{
	// TODO Auto-generated destructor stub
}

bool RMCSUsbIface::init(uint8_t number,
						uint8_t altSetting,
						const UsbStringDescriptor & ifaceStr)
{
	return XUsbIface::init(number, altSetting, 0xFF,
						   RMCS_USB_DEVICE_CLASS, 0, ifaceStr);
}

bool RMCSUsbIface::setupRequest(UsbSetupRequest * req)
{
    if((req->bmRequest & 0x80) == 0)
    //! CONTROL OUT
    {
        _setupRequest = req;
        if(req->wLength > 0)
            ep0Receive(_setupDataBuffer, req->wLength);
        else
            controlPacketReceived(req->wValue, req->bRequest, ControlPacket(0, 0));
    }
    //! CONTROL IN
    else
    {
        ControlPacket packet(_setupDataBuffer, MAX_SETUP_DATA_LENGTH);
        if(controlPacketRequested(req->wValue, req->bRequest, packet))
            ep0Transmit(packet.data(), packet.packetSize());
    }

    return true;
}

void RMCSUsbIface::ep0RxReady(UsbSetupRequest * req)
{
	controlPacketReceived(req->wValue, req->bRequest,
						  ControlPacket(_setupDataBuffer, MAX_SETUP_DATA_LENGTH));
}

void RMCSUsbIface::ep0TxSent(UsbSetupRequest * req)
{
	//! DO NOTHING!
}

