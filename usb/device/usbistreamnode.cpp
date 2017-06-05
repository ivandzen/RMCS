#include "usbistreamnode.h"
#include "rmcsusbdevice.h"
#include <usb/common/common.h>

UsbIStreamNode::UsbIStreamNode(const char * name,
                               uint8_t ep_num,
                               Length_t reserve,
                               RMCSUsbDevice * device) :
    IStreamNode(name,
                NODE_TYPE_USBISTREAM,
                reserve,
                device->rmcsDevice()),
    UsbDeviceEP(UsbDeviceEP::EP_ISOC,
                ep_num & 0x7F,
                EP_ATTRIBUTES,
                1, device)
{
	device->setIStream(this);
}

UsbDeviceEP::Status UsbIStreamNode::dataOut()
{
    uint8_t ready_index = _inPacketCounter;
    _inPacketCounter++;
    if(_inPacketCounter == numPackets())
        _inPacketCounter = 0;
    if(isStreamEnabled())
        prepareReceive(istreamPacket(_inPacketCounter), packetSize());
    streamDataReceived(istreamPacket(ready_index), packetSize());
    return OK;
}

bool UsbIStreamNode::settingsRequested(ControlPacket & packet) const
{
    return UsbStreamSettings(packet).init(epAddr(),
                                          packetSize());
}

bool UsbIStreamNode::init()
{
    if(!IStreamNode::init())
        return false;
    setMPS(packetSize());
    open();
    return true;
}

void UsbIStreamNode::streamToggled(bool enabled)
{
    if(enabled)
    {
        _inPacketCounter = 0;
        prepareReceive(istreamPacket(_inPacketCounter), packetSize());
    }
}
