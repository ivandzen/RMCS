#ifndef USBOSTREAMNODE_H
#define USBOSTREAMNODE_H
#include <core/device/ostreamnode.h>
#include <usb/device/usbdeviceep.h>

class RMCSUsbDevice;

class UsbOStreamNode :
        public OStreamNode,
        public UsbDeviceEP
{
public:
    UsbOStreamNode(const char * name,
                   uint8_t ep_num,
                   RMCSUsbDevice * device);

    virtual Status dataIn() override;

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool init() override;

protected:
    virtual void streamToggled(bool enabled) final override;

private:
    std::vector<Data> _buffer;
};


#endif //USBOSTREAMNODE_H
