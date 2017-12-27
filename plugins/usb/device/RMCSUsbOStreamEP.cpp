/*
 * RMCSUsbOStreamEP.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#include <plugins/usb/device/RMCSUsbOStreamEP.h>
#include <plugins/usb/device/RMCSUsbIface.h>
#include <plugins/usb/common/common.h>

RMCSUsbOStreamEP::RMCSUsbOStreamEP(const XUsbEndpoint & source,
		 	 	 	 	 	 	   const char * name,
								   uint8_t epnum,
								   uint16_t mps) :
		OStreamNode(name, NODE_TYPE_USBOSTREAM,
					static_cast<RMCSUsbIface*>(source.iface()), mps),
		XUsbInEndpoint(source)
{
	XUsbInEndpoint::init(UsbEPDescriptor::DEFAULT_LENGTH, 0x80 | epnum, USBISTREAM_EP_ATTRIBUTES, mps, 1);
}

bool RMCSUsbOStreamEP::settingsRequested(ControlPacket & packet) const
{
	return UsbStreamSettings(packet).init(bEndpointAddress(),
	                                      packetSize());
}

void RMCSUsbOStreamEP::streamToggled(bool enabled)
{
    if(enabled)
        transmit(ostreamPacket(), packetSize());
}

bool RMCSUsbOStreamEP::epDataIn(uint8_t * pdata)
{
    if(!isStreamEnabled())
        return true;
    transmit(ostreamPacket(), packetSize());
    return true;
}
