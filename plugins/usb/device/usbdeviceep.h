#ifndef USBDEVICEEP_H
#define USBDEVICEEP_H
#include <stdint.h>
#include <plugins/usb/device/usbdescriptors.h>
#include <usb_otg.h>

class UsbDevice;

class UsbDeviceEP
{
	friend class UsbDevice;

public:
    enum Type
    {
        EP_CTRL         = 0,
        EP_ISOC         = 1,
        EP_BULK         = 2,
        EP_INTR         = 3
    };

    enum Status
    {
        OK,
        BUSY,
        FAIL
    };

    UsbDeviceEP(Type type,
                uint8_t addr,
                uint8_t attributes,
                uint8_t iso_interval,
                UsbDevice * device);

    virtual ~UsbDeviceEP() {}

    inline void setMPS(uint16_t value) { _mps = value; }

    inline uint8_t epAddr() const { return _addr; }

    inline EndpDesc getDescriptor() const
    {
        EndpDesc desc;
        desc.setByte(EndpDesc::bEndpointAddress, _addr);
        desc.setByte(EndpDesc::bmAttributes, _attributes);
        desc.setWord(EndpDesc::wMaxPacketSize, _mps);
        desc.setByte(EndpDesc::bInterval, _isoInterval);
        return desc;
    }

    inline Status open()
    {
        HAL_PCD_EP_Open(_devHandle, _addr, _mps, _type);
        return OK;
    }

    inline Status close()
    {
        HAL_PCD_EP_Close(_devHandle, _addr);
        return OK;
    }

    inline Status flush()
    {
        HAL_PCD_EP_Flush(_devHandle, _addr);
        return OK;
    }

    inline void stall()
    {
        HAL_PCD_EP_SetStall(_devHandle, _addr);
    }

    inline Status clearStall()
    {
        HAL_PCD_EP_ClrStall(_devHandle, _addr);
        return OK;
    }

    inline uint8_t isStall() const
    {
        if((_addr & 0x80) == 0x80)
        {
            return _devHandle->IN_ep[_addr & 0x7F].is_stall;
        }
        else
        {
            return _devHandle->OUT_ep[_addr & 0x7F].is_stall;
        }
    }

    inline Status prepareReceive(uint8_t * pbuf, uint16_t size)
    {
        HAL_PCD_EP_Receive(_devHandle,
                           _addr,
                           pbuf,
                           size);
        return OK;
    }

    inline Status transmit(uint8_t * pbuf, uint16_t size)
    {
        HAL_PCD_EP_Transmit(_devHandle,
                            _addr,
                            pbuf,
                            size);
        return OK;
    }

protected:
    virtual Status dataOut() { return OK; }

    virtual Status dataIn() { return OK; }

    virtual Status isoInIncomplete() { return OK; }

    virtual Status isoOutIncomplete() { return OK; }

private:
    Type                _type;
    uint8_t             _addr;
    uint16_t            _mps;
    uint8_t             _attributes;
    uint8_t             _isoInterval;
    PCD_HandleTypeDef * _devHandle;
    uint16_t            _status;
};

#endif // USBDEVICEEP_H
