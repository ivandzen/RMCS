#ifndef USBISTREAMNODE_H
#define USBISTREAMNODE_H
#include <core/device/istreamnode.h>
#include <usb/device/usbdeviceep.h>

class RMCSUsbDevice;

class UsbIStreamNode :
        public IStreamNode,
        public UsbDeviceEP
{
public:
    UsbIStreamNode(const char * name,
                   uint8_t ep_num,
                   Length_t reserve,
                   RMCSUsbDevice * device);

    virtual Status dataOut() override;

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool init() override;

protected:
    virtual void streamToggled(bool enabled) final override;

private:
    std::vector<Data> _buffer;
    uint8_t    _inPacketCounter;
};

#endif //USBISTREAMNODE_H
