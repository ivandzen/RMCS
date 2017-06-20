#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <rmcs_device_conf.h>
#include <plugins/usb/device/usbdescriptors.h>
#include <plugins/usb/device/usbdeviceep.h>

#define USB_DEVICE_CAPABITY_TYPE                           0x10

#define USB_HS_MAX_PACKET_SIZE                            512
#define USB_FS_MAX_PACKET_SIZE                            64
#define USB_MAX_EP0_SIZE                                  64


#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8))

#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

#ifdef __cplusplus
extern "C" {
#endif

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd);
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd);
#if (USBD_LPM_ENABLED == 1)
void HAL_PCDEx_LPM_Callback(PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg);
#endif

#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////

class UsbDevice {
    friend void ::HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
    friend void ::HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
    friend void ::HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
    friend void ::HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum);
    friend void ::HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd);
    friend void ::HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd);
#if (USBD_LPM_ENABLED == 1)
    friend void HAL_PCDEx_LPM_Callback(PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg);
#endif

public:
    struct SetupRequest {
        uint8_t   bmRequest;
        uint8_t   bRequest;
        uint16_t  wValue;
        uint16_t  wIndex;
        uint16_t  wLength;
    };

#pragma pack(push, 1)

    enum DeviceState {
        DEV_DEFAULT,
        DEV_ADDRESSED,
        DEV_CONFIGURED,
        DEV_SUSPENDED
    };

    enum Status {
        OK,
        BUSY,
        FAIL
    };

    enum Speed {
        USBD_SPEED_HIGH  = 0,
        USBD_SPEED_FULL  = 1,
        USBD_SPEED_LOW   = 2,
    };

    enum EPType {
        EP_CTRL         = 0,
        EP_ISOC         = 1,
        EP_BULK         = 2,
        EP_INTR         = 3
    };

    enum IsoEPSyncMode {
        SYNC_MODE_NONE                             = 0,
        SYNC_MODE_ASYNC                            = (1 << 2),
        SYNC_MODE_ADAPTIVE                         = (2 << 2),
        SYNC_MODE_SYNC                             = (3 << 2)
    };

    enum IsoEPUsageMode {
        USAGE_MODE_DATA                            = 0,
        USAGE_MODE_FEEDBACK                        = (1 << 4),
        USAGE_MODE_EXFBDATA                        = (2 << 4),
        USAGE_MODE_RESERVED                        = (3 << 4)
    };

#pragma pack(pop)

    Status start();

    Status stop();

    inline DeviceState state() const { return _dev_state; }

    inline PCD_HandleTypeDef * handle() const { return _pcd_handle; }

protected:

#pragma pack(push, 1)

    enum UsbReqType {
        REQ_TYPE_STANDARD                          = 0x00,
        REQ_TYPE_CLASS                             = 0x20,
        REQ_TYPE_VENDOR                            = 0x40,
        REQ_TYPE_MASK                              = 0x60
    };

    enum UsbRequest {
        REQ_GET_STATUS                             = 0x00,
        REQ_CLEAR_FEATURE                          = 0x01,
        REQ_SET_FEATURE                            = 0x03,
        REQ_SET_ADDRESS                            = 0x05,
        REQ_GET_DESCRIPTOR                         = 0x06,
        REQ_SET_DESCRIPTOR                         = 0x07,
        REQ_GET_CONFIGURATION                      = 0x08,
        REQ_SET_CONFIGURATION                      = 0x09,
        REQ_GET_INTERFACE                          = 0x0A,
        REQ_SET_INTERFACE                          = 0x0B,
        REQ_SYNCH_FRAME                            = 0x0C
    };

#pragma pack(pop)

    UsbDevice(PCD_HandleTypeDef * pcd_handle,
              const char * manufacturer_str,
              const char * product_str,
              const char * serial_str,
              uint8_t max_num_itf,
              uint8_t max_num_cfg);

    virtual ~UsbDevice();

    inline void setBcdUSB(DeviceDesc::BCDUsb value) {
        _devDesc.setWord(DeviceDesc::bcdUSB, value);
        _devDesc.setWord(DeviceDesc::bcdDevice, value);
    }
    inline void setDeviceClass(uint8_t value) { _devDesc.setByte(DeviceDesc::bDeviceClass, value); }
    inline void setDeviceSubClass(uint8_t value) { _devDesc.setByte(DeviceDesc::bDeviceSubClass, value); }
    inline void setDeviceProtocol(uint8_t value) { _devDesc.setByte(DeviceDesc::bDeviceProtocol, value); }
    inline void setEp0PacketSize(DeviceDesc::EP0PacketSize value) { _devDesc.setByte(DeviceDesc::bMaxPacketSize, value); }
    inline void setVendorID(uint16_t value) { _devDesc.setWord(DeviceDesc::idVendor, value); }
    inline void setProductID(uint16_t value) { _devDesc.setWord(DeviceDesc::idProduct, value); }

#if (USBD_LPM_ENABLED == 1)
    virtual uint8_t * getBOSDescriptor(Speed speed, uint16_t * len) = 0;
#endif
    uint8_t * getDeviceDescriptor(Speed, uint16_t * len);
    uint8_t * getLangIDStrDescriptor(Speed, uint16_t * len);
    uint8_t * getManufacturerStrDescriptor(Speed, uint16_t * len);
    uint8_t * getProductStrDescriptor(Speed, uint16_t * len);
    uint8_t * getSerialStrDescriptor(Speed, uint16_t * len);
    virtual uint8_t * getHSConfigDescriptor(uint16_t * len) = 0;
    virtual uint8_t * getFSConfigDescriptor(uint16_t * len) = 0;
    virtual uint8_t * getConfigurationStrDescriptor(Speed speed, uint16_t * len) = 0;
    virtual uint8_t * getInterfaceStrDescriptor(Speed speed, uint16_t * len) = 0;
    virtual uint8_t * getUsrStrDescriptor(uint16_t wValue, uint16_t * len) = 0;
    virtual uint8_t * getDeviceQualifierDescriptor(uint16_t * len) = 0;
    virtual uint8_t * getOtherSpeedConfigDescriptor(uint16_t * len) = 0;

    virtual void   onDeviceStarted() {}

    virtual Status classInit(uint8_t cfgidx) = 0;
    virtual Status classDeInit(uint8_t cfgindx) = 0;
    virtual Status classSetup(SetupRequest * req) = 0;
    virtual Status classEP0_RxReady() = 0;
    virtual Status classEP0_TxSent() = 0;
    virtual Status classDataOut(uint8_t epnum) = 0;
    virtual Status classDataIn(uint8_t epnum) = 0;
    virtual Status classSOF() = 0;
    virtual Status classIsoOutIncomplete(uint8_t epnum) = 0;
    virtual Status classIsoInIncomplete(uint8_t epnum) = 0;

    Status      prepareReceive(uint8_t ep_addr, uint8_t *pbuf, uint16_t size);
    void        ctlError(SetupRequest * req);
    Status      ctlSendData(uint8_t * pbuf, uint16_t len);
    Status      ctlPrepareRx(uint8_t * pbuf, uint16_t len);
    Status      transmit(uint8_t ep_addr, uint8_t * pbuf, uint16_t size);
    Status      openEP(uint8_t ep_addr, EPType ep_type, uint16_t ep_mps);
    Status      closeEP(uint8_t ep_addr);
    Status      flushEP(uint8_t ep_addr);
    Status      stallEP(uint8_t epnum);
    Status      clearStallEP(uint8_t epnum);
    uint8_t     isStallEP(uint8_t epnum);
    uint32_t    getRxDataSize(uint8_t ep_addr);
    void        delay(uint32_t value);

private:

#pragma pack(push, 1)

    enum DevConfig {
        CONFIG_REMOTE_WAKEUP    = 2,
        CONFIG_SELF_POWERED     = 1
    };

    enum Feature {
        FEATURE_EP_HALT         = 0,
        FEATURE_REMOTE_WAKEUP   = 1,
        FEATURE_TEST_MODE       = 2
    };

    enum EP0State {
        EP0_IDLE,
        EP0_SETUP,
        EP0_DATA_IN,
        EP0_DATA_OUT,
        EP0_STATUS_IN,
        EP0_STATUS_OUT,
        EP0_STALL
    };

    enum UsbReqRecipient {
        REQ_RECIPIENT_DEVICE                       = 0x00,
        REQ_RECIPIENT_INTERFACE                    = 0x01,
        REQ_RECIPIENT_ENDPOINT                     = 0x02,
        REQ_RECIPIENT_MASK                         = 0x03
    };

#pragma pack(pop)

    static const uint8_t LANGID_STR_DESC_INDEX = 0;
    static const uint8_t MANUFACTURER_STR_DESC_INDEX = 1;
    static const uint8_t PRODUCT_STR_DESC_INDEX = 2;
    static const uint8_t SERIAL_STR_DESC_INDEX = 3;
    static const uint8_t CONFIG_STR_DESC_INDEX = 4;
    static const uint8_t INTERFACE_STR_DESC_INDEX = 5;


    struct Endpoint
    {
        uint32_t                status;
        uint32_t                total_length;
        uint32_t                rem_length;
        uint32_t                maxpacket;
    };


    Status  setupStage(uint8_t * setup);
    Status  dataOutStage(uint8_t epnum, uint8_t * pdata);
    Status  dataInStage(uint8_t epnum, uint8_t * pdata);
    Status  SOF();
    Status  suspend();
    Status  resume();
    Status  isoOutIncomplete(uint8_t epnum);
    Status  isoInIncomplete(uint8_t epnum);
    Status  connected();
    Status  disconnected();
    Status  setSpeed(Speed speed);
    Status  reset();

    void    parseSetupRequest(uint8_t * pdata);
    Status  stdDevReq(SetupRequest * req);
    Status  stdItfReq(SetupRequest * req);
    Status  stdEPReq(SetupRequest * req);

    void    getDescriptor (SetupRequest * req) ;
    void    setAddress(SetupRequest * req);
    void    setConfig (SetupRequest * req);
    void    getConfig (SetupRequest * req);
    void    getStatus (SetupRequest * req);
    void    setFeature (SetupRequest * req);
    void    clrFeature (SetupRequest * req);

    Status  setUSBAddress(uint8_t dev_addr);
    Status  ctlSendStatus();
    Status  ctlContinueRx(uint8_t * pbuf, uint16_t len);
    Status  ctlContinueSendData(uint8_t * pbuf, uint16_t len);
    Status  ctlReceiveStatus();
    Status  runTestMode();

    //! Дескрипторы
    DeviceDesc          _devDesc;
    LangIDDesc          _langIDDesc;
    StringDesc          _manufacturerStrDesc;
    StringDesc          _productStrDesc;
    StringDesc          _serialStrDesc;

    PCD_HandleTypeDef * _pcd_handle;
    uint32_t            _dev_config;
    uint32_t            _dev_default_config;
    uint32_t            _dev_config_status;
    Speed               _dev_speed;
    Endpoint            _ep_in[15];
    Endpoint            _ep_out[15];
    EP0State            _ep0_state;
    uint32_t            _ep0_data_len;
    DeviceState         _dev_state;
    DeviceState         _dev_old_state;
    uint8_t             _dev_address;
    uint8_t             _dev_connection_status;
    uint8_t             _dev_test_mode;
    uint32_t            _dev_remote_wakeup;
    SetupRequest        _request;
    uint8_t             _max_num_configuration;
    uint8_t             _max_num_interfaces;
};

#endif // USBDEVICE_H
