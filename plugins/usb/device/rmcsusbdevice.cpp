#include <plugins/usb/device/rmcsusbdevice.h>
#include <core/common/control_protocol.h>

#ifdef ENABLE_USBDEV

#define USB_SIZ_BOS_DESC                0x0C

#define USBD_LPM_ENABLED 0

#if (USBD_LPM_ENABLED == 1)
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4
#endif
__ALIGN_BEGIN  uint8_t ISOUSB_FS_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
  0x5,
  USB_DESC_TYPE_BOS,
  0xC,
  0x0,
  0x1,  /* 1 device capability */
        /* device capability*/
  0x7,
  USB_DEVICE_CAPABITY_TYPE,
  0x2,
  0x2, /*LPM capability bit set */
  0x0,
  0x0,
  0x0
};
#endif

#define ISOUSB_ISOOUT_EP_INTERVAL   1
#define ISOUSB_ISOIN_EP_INTERVAL    1

#define ISOUSB_ISO_IN_EP_ATTRIBUTES (UsbDevice::EP_ISOC | \
                                        UsbDevice::SYNC_MODE_NONE | \
                                        UsbDevice::USAGE_MODE_DATA)

#define ISOUSB_ISO_OUT_EP_ATTRIBUTES (UsbDevice::EP_ISOC | \
                                        UsbDevice::SYNC_MODE_NONE | \
                                        UsbDevice::USAGE_MODE_DATA)

RMCSUsbDevice::RMCSUsbDevice(Device * rmcs_device,
							 PCD_HandleTypeDef * pcd_handle,
							 const char * manufacturer_str,
							 const char * device_name_str,
							 const char * serial_name_str) :
    UsbDevice(pcd_handle, manufacturer_str, device_name_str, serial_name_str, 1, 1),
    _rmcsDevice(rmcs_device),
    _istream(nullptr),
    _ostream(nullptr),
    _configStrDesc(StringDesc("ISO USB Hand")),
    _ifaceStrDesc(StringDesc("ISO USB Interface")),
    _setupRequest(0),
	_inputPacket(ControlPacket(_setupDataBuffer, MAX_SETUP_DATA_LENGTH))
#if (USBD_LPM_ENABLED == 1)
    ,_BOSDesc(BOSDesc());
#endif
{
    setVendorID(ISOUSB_VID);
    setProductID(ISOUSB_PID);
}

RMCSUsbDevice::~RMCSUsbDevice()
{
}

#if (USBD_LPM_ENABLED == 1)
uint8_t *RMCSUsbDevice::getBOSDescriptor(UsbDevice::Speed speed, uint16_t *len)
{

}
#endif

uint8_t *RMCSUsbDevice::getHSConfigDescriptor(uint16_t *len)
{
    *len = _configDesc.length();
    return *_configDesc;
}

uint8_t *RMCSUsbDevice::getFSConfigDescriptor(uint16_t *len)
{
    *len = _configDesc.length();
    return *_configDesc;
}

uint8_t *RMCSUsbDevice::getConfigurationStrDescriptor(UsbDevice::Speed,
													  uint16_t *len)
{
    *len = _configStrDesc.length();
    return *_configStrDesc;
}

uint8_t *RMCSUsbDevice::getInterfaceStrDescriptor(UsbDevice::Speed,
												  uint16_t *len)
{
    *len = _ifaceStrDesc.length();
    return *_ifaceStrDesc;
}

uint8_t *RMCSUsbDevice::getUsrStrDescriptor(uint16_t,
										    uint16_t *len)
{
    *len = 0;
    return 0;
}

uint8_t *RMCSUsbDevice::getDeviceQualifierDescriptor(uint16_t *len)
{
    *len = 0;
    return 0;
}

uint8_t *RMCSUsbDevice::getOtherSpeedConfigDescriptor(uint16_t *len)
{
    *len = _configDesc.length();
    return *_configDesc;
}

UsbDevice::Status RMCSUsbDevice::classInit(uint8_t)
{
    if(_istream != nullptr) _istream->open();
    if(_ostream != nullptr) _ostream->open();
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classDeInit(uint8_t)
{
	if(_istream != nullptr) _istream->close();
	if(_ostream != nullptr) _ostream->close();
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classSetup(UsbDevice::SetupRequest *req)
{
    static uint8_t ifalt = 0;
    UsbDevice::Status ret = OK;

    switch (req->bmRequest & REQ_TYPE_MASK)
    {
    case REQ_TYPE_CLASS :
        ret = classSetupRequest(req);
        break;

    case REQ_TYPE_STANDARD:
        switch (req->bRequest)
        {
        case REQ_GET_INTERFACE :
            ctlSendData (&ifalt, 1);
            break;

        case REQ_SET_INTERFACE :
        {
            break;
        }
        }
    }
    return ret;
}

UsbDevice::Status RMCSUsbDevice::classEP0_RxReady()
{
    _rmcsDevice->controlPacketReceived(_setupRequest->wValue,
                                       _setupRequest->bRequest,
                                       _inputPacket);
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classEP0_TxSent()
{
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classDataOut(uint8_t)
{
    if(_istream != nullptr) _istream->dataOut();
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classDataIn(uint8_t)
{
    if(_ostream != nullptr) _ostream->dataIn();
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classSOF()
{
	_istream->startOfFrame();
	_ostream->startOfFrame();
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classIsoOutIncomplete(uint8_t)
{
    return OK;
}

UsbDevice::Status RMCSUsbDevice::classIsoInIncomplete(uint8_t)
{
    return OK;
}

void RMCSUsbDevice::onDeviceStarted()
{
    IfaceDesc iface;
    iface.setByte(IfaceDesc::bInterfaceNumber, ISOUSB_BASE_INTERFACE);
    iface.setByte(IfaceDesc::bAlternateSetting, 0);
    iface.setByte(IfaceDesc::bInterfaceClass, 0xFF);
    iface.setByte(IfaceDesc::bInterfaceSubClass, 0x00);
    iface.setByte(IfaceDesc::bInterfaceProtocol, 0x00);
    iface.setByte(IfaceDesc::iInterface, 0x00);

    if(_istream != nullptr)
    {
    	EndpDesc isooutep = _istream->getUsbDescriptor();
    	iface.addEndpoint(isooutep);
    }

    if(_ostream != nullptr)
    {
    	EndpDesc isoinep = _ostream->getUsbDescriptor();
    	iface.addEndpoint(isoinep);
    }

	_configDesc.setByte(ConfigDesc::bConfigurationValue, ISOUSB_BASE_CONFIG);
    _configDesc.setByte(ConfigDesc::iConfiguration, 0x00);
    _configDesc.setByte(ConfigDesc::bmAttributes, 0xC0);
    _configDesc.setByte(ConfigDesc::bMaxPower, 0x32);
    _configDesc.addInterface(iface);
}

UsbDevice::Status RMCSUsbDevice::classSetupRequest(UsbDevice::SetupRequest *req)
{
    if((req->bmRequest & 0x80) == 0) { //! CONTROL OUT
        _setupRequest = req;
        if(req->wLength > 0)
            ctlPrepareRx(_setupDataBuffer, req->wLength);
        else
            _rmcsDevice->controlPacketReceived(req->wValue, req->bRequest, ControlPacket(0, 0));
    }
    else {                           //! CONTROL IN
        ControlPacket packet(_setupDataBuffer, MAX_SETUP_DATA_LENGTH);
        if(_rmcsDevice->controlPacketRequested(req->wValue, req->bRequest, packet))
            ctlSendData(packet.data(), packet.packetSize());
    }

    return OK;
}

#endif
