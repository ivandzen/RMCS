#ifndef USBOSTREAMNODE_H
#define USBOSTREAMNODE_H

#ifdef ENABLE_USBDEVICE

#include <core/device/ostreamnode.h>
#include <plugins/usb/device/usbdeviceep.h>

class RMCSUsbDevice;

class UsbOStreamNode :
        public OStreamNode,
        public UsbDeviceEP
{
public:
    UsbOStreamNode(const char * name,
                   uint8_t ep_num,
                   RMCSUsbDevice * device,
				   uint16_t mps);

    virtual Status dataIn() override;

    void startOfFrame();

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool init() override;

protected:
    virtual void streamToggled(bool enabled) final override;

private:
    std::vector<Data> _buffer;
};

#endif

#endif //USBOSTREAMNODE_H
