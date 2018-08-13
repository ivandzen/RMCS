/*
 * RMCSUsbIStreamEP.cpp
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#include <plugins/usb/device/RMCSUsbIStreamEP.h>
#include <plugins/usb/device/RMCSUsbIface.h>
#include <plugins/usb/common/common.h>

RMCSUsbIStreamEP::RMCSUsbIStreamEP(const XUsbEndpoint & source,
								   const char * name,
								   Length_t num_packets,
								   uint8_t epnum,
								   uint16_t mps) :
		IStreamNode(name, NODE_TYPE_USBISTREAM, num_packets,
					static_cast<RMCSUsbIface*>(source.iface()), mps),
		XUsbOutEndpoint(source),
		_inPacketCounter(0)
{
	XUsbOutEndpoint::init(UsbEPDescriptor::DEFAULT_LENGTH, epnum, USBSTREAM_EP_ATTRIBUTES, mps, 0);
}

bool RMCSUsbIStreamEP::settingsRequested(ControlPacket & packet) const
{
	return UsbStreamSettings(packet).init(bEndpointAddress(),
	                                      packetSize());
}

void RMCSUsbIStreamEP::streamToggled(bool enabled)
{
    if(enabled)
    {
        _inPacketCounter = 0;
        receive(istreamPacket(_inPacketCounter), packetSize());
    }
}

bool RMCSUsbIStreamEP::epDataOut(uint8_t *)
{
	uint8_t ready_index = _inPacketCounter;
	_inPacketCounter++;
	if(_inPacketCounter == numPackets())
	    _inPacketCounter = 0;
	if(isStreamEnabled())
	    receive(istreamPacket(_inPacketCounter), packetSize());
	streamDataReceived(istreamPacket(ready_index), packetSize());
	return true;
}

