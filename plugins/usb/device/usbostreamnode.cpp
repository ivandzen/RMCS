#include "usbostreamnode.h"
#include "rmcsusbdevice.h"

UsbOStreamNode::UsbOStreamNode(const char * name,
                               uint8_t ep_num,
                               RMCSUsbDevice * device) :
    OStreamNode(name,
                NODE_TYPE_USBOSTREAM,
                device->rmcsDevice()),
    UsbDeviceEP(UsbDeviceEP::EP_ISOC,
                ep_num | 0x80,
                EP_ATTRIBUTES,
                1, device)
{
	device->setOStream(this);
}

UsbDeviceEP::Status UsbOStreamNode::dataIn()
{
    if(!isStreamEnabled())
        return OK;
    transmit(ostreamPacket(), packetSize());
    return OK;
}

bool UsbOStreamNode::settingsRequested(ControlPacket & packet) const
{
    return UsbStreamSettings(packet).init(epAddr(),
                                          packetSize());
}

bool UsbOStreamNode::init()
{
    if(!OStreamNode::init())
        return false;
    setMPS(packetSize());
    open();
    return true;
}

void UsbOStreamNode::streamToggled(bool enabled)
{
    if(enabled)
        transmit(ostreamPacket(), packetSize());
}