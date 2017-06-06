#ifndef RMCSUSBDEVICE_H
#define RMCSUSBDEVICE_H

#include <usb/common/common.h>
#include <usb/device/usbdevice.h>
#include <core/device/device.h>
#include <usb/device/usbistreamnode.h>
#include <usb/device/usbostreamnode.h>

class RMCSUsbDevice :
        public UsbDevice
{

    friend class IsoUsbNode;

public:
    RMCSUsbDevice(Device * rmcs_device,
    			  PCD_HandleTypeDef * pcd_handle,
    			  const char * manufacturer_str,
    			  const char * device_name_str,
				  const char * serial_name_str);

    virtual ~RMCSUsbDevice();

    inline void setIStream(UsbIStreamNode * istream) { _istream = istream; }

    inline void setOStream(UsbOStreamNode * ostream) { _ostream = ostream; }

    inline Device * rmcsDevice() { return _rmcsDevice; }

protected:
#if (USBD_LPM_ENABLED == 1)
    virtual uint8_t * getBOSDescriptor(Speed speed, uint16_t * len);
#endif
    virtual uint8_t *           getHSConfigDescriptor(uint16_t * len) final override;
    virtual uint8_t *           getFSConfigDescriptor(uint16_t * len) final override;
    virtual uint8_t *           getConfigurationStrDescriptor(Speed, uint16_t * len) final override;
    virtual uint8_t *           getInterfaceStrDescriptor(Speed, uint16_t * len) final override;
    virtual uint8_t *           getUsrStrDescriptor(uint16_t, uint16_t * len) final override;
    virtual uint8_t *           getDeviceQualifierDescriptor(uint16_t * len) final override;
    virtual uint8_t *           getOtherSpeedConfigDescriptor(uint16_t * len) final override;


    virtual Status              classInit(uint8_t) override;
    virtual Status              classDeInit(uint8_t) override;
    virtual Status              classSetup(SetupRequest * req) final override;
    virtual Status              classEP0_RxReady() final override;
    virtual Status              classEP0_TxSent() final override;
    virtual Status              classDataOut(uint8_t epnum) final override;
    virtual Status              classDataIn(uint8_t epnum) final override;
    virtual Status              classSOF() final override;
    virtual Status              classIsoOutIncomplete(uint8_t) final override;
    virtual Status              classIsoInIncomplete(uint8_t ) final override;

    virtual void                onDeviceStarted() override;

private:
    Status                      classSetupRequest(SetupRequest * req);

    Device *                    _rmcsDevice;

    UsbIStreamNode *            _istream;
    UsbOStreamNode *            _ostream;
    ConfigDesc                  _configDesc;
    StringDesc                  _configStrDesc;
    StringDesc                  _ifaceStrDesc;

    //! поля хранят данные сетап реквестов для нод
    SetupRequest *              _setupRequest;
    uint8_t                     _setupDataBuffer[MAX_SETUP_DATA_LENGTH];
    ControlPacket				_inputPacket;

#if (USBD_LPM_ENABLED == 1)
    BOSDesc             _BOSDesc;
#endif
};

#endif //RMCSUSBDEVICE_H
