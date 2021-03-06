/*
 * RMCSUsbOStreamEP.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#include <plugins/usb/device/RMCSUsbOStreamEP.h>
#include <plugins/usb/device/RMCSUsbIface.h>
#include <plugins/usb/common/common.h>

#define USBSTREAM_EP_ATTRIBUTES (UsbEPDescriptor::EP_BULK | \
                       	   	   	 UsbEPDescriptor::SYNC_MODE_NONE | \
								 UsbEPDescriptor::USAGE_MODE_DATA)

RMCSUsbOStreamEP::RMCSUsbOStreamEP(const XUsbEndpoint & source,
		 	 	 	 	 	 	   const char * name,
								   uint8_t epnum,
								   uint16_t mps) :
		OStreamNode(name, NODE_TYPE_USBOSTREAM,
					static_cast<RMCSUsbIface*>(source.iface()), mps),
		XUsbInEndpoint(source)
{
	XUsbInEndpoint::init(UsbEPDescriptor::DEFAULT_LENGTH, epnum, USBSTREAM_EP_ATTRIBUTES, mps, 0);
}

bool RMCSUsbOStreamEP::settingsRequested(ControlPacket & packet) const
{
	return UsbStreamSettings(packet).init(bEndpointAddress(),
	                                      bufferSize());
}

void RMCSUsbOStreamEP::streamToggled(bool enabled)
{
    if(enabled)
        transmit(ostreamPacket(), bufferSize());
}

void RMCSUsbOStreamEP::sync()
{
	//if(isStreamEnabled())
	//	transmit(ostreamPacket(), bufferSize());
}

bool RMCSUsbOStreamEP::epDataIn(uint8_t *)
{
    if(isStreamEnabled())
    	transmit(ostreamPacket(), bufferSize());
    return true;
}

