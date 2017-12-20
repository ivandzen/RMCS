#include "usbdevice.h"

#if ENABLE_USBDEV

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Setup stage callback
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->setupStage((uint8_t*)hpcd->Setup);
}

/**
  * @brief  Data Out stage callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->dataOutStage(epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
  * @brief  Data In stage callback..
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->dataInStage(epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
  * @brief  SOF callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->SOF();
}

/**
  * @brief  Reset callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    UsbDevice::Speed speed = UsbDevice::USBD_SPEED_FULL;

    /*Set USB Current Speed*/
    switch (hpcd->Init.speed) {
    case PCD_SPEED_HIGH:
        speed = UsbDevice::USBD_SPEED_HIGH;
        break;
    case PCD_SPEED_FULL:
        speed = UsbDevice::USBD_SPEED_FULL;
        break;

    default:
        speed = UsbDevice::USBD_SPEED_FULL;
        break;
    }

    device->reset(speed);
}

/**
  * @brief  Suspend callback.
  * When Low power mode is enabled the debug cannot be used (IAR, Keil doesn't support it)
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    /* Inform USB library that core enters in suspend Mode */
    device->suspend();
    __HAL_PCD_GATE_PHYCLOCK(hpcd);
    /*Enter in STOP mode */
    /* USER CODE BEGIN 2 */
    if (hpcd->Init.low_power_enable) {
        /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
        SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
    }
    /* USER CODE END 2 */
}

/**
  * @brief  Resume callback.
    When Low power mode is enabled the debug cannot be used (IAR, Keil doesn't support it)
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->resume();
}

/**
  * @brief  ISOC Out Incomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->isoOutIncomplete(epnum);
}

/**
  * @brief  ISOC In Incomplete callback.
  * @param  hpcd: PCD handle
  * @param  epnum: Endpoint Number
  * @retval None
  */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->isoInIncomplete(epnum);
}

/**
  * @brief  Connect callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->connected();
}

/**
  * @brief  Disconnect callback.
  * @param  hpcd: PCD handle
  * @retval None
  */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    device->disconnected();
}


#if (USBD_LPM_ENABLED == 1)
/**
  * @brief  HAL_PCDEx_LPM_Callback : Send LPM message to user layer
  * @param  hpcd: PCD handle
  * @param  msg: LPM message
  * @retval HAL status
  */
void HAL_PCDEx_LPM_Callback(PCD_HandleTypeDef *hpcd, PCD_LPM_MsgTypeDef msg) {
    UsbDevice * device = (UsbDevice*)hpcd->pData;
    switch ( msg) {
    case PCD_LPM_L0_ACTIVE:
        if (hpcd->Init.low_power_enable) {
            SystemClock_Config();

            /* Reset SLEEPDEEP bit of Cortex System Control Register */
            SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
        }
        __HAL_PCD_UNGATE_PHYCLOCK(hpcd);
        device->resume();
        break;

    case PCD_LPM_L1_ACTIVE:
        __HAL_PCD_GATE_PHYCLOCK(hpcd);
        device->suspend();

        /*Enter in STOP mode */
        if (hpcd->Init.low_power_enable) {
            /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register */
            SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
        }
        break;
  }
}
#endif


#ifdef __cplusplus
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::start()
{
    if(HAL_PCD_Start(_pcd_handle) == HAL_OK) {
        onDeviceStarted();
        return OK;
    }
    return FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::stop()
{
    if(HAL_PCD_Stop(_pcd_handle) == HAL_OK)
        return OK;
    return FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::UsbDevice(PCD_HandleTypeDef * pcd_handle,
                     const char * manufacturer_str,
                     const char * product_str,
                     const char * serial_str,
                     uint8_t max_num_itf,
                     uint8_t max_num_cfg) :
    _langIDDesc(LangIDDesc(1033)), //! @attention в будущем сделать параметр для конструктора
    _manufacturerStrDesc(StringDesc(manufacturer_str)),
    _productStrDesc(StringDesc(product_str)),
    _serialStrDesc(StringDesc(serial_str)),
    _pcd_handle(pcd_handle),
    _dev_config(0),
    _dev_default_config(0),
    _dev_state(DEV_DEFAULT),
    _max_num_configuration(max_num_cfg),
    _max_num_interfaces(max_num_itf)
{
    pcd_handle->pData = this;
    HAL_PCDEx_SetRxFiFo(pcd_handle, 0x80);
    HAL_PCDEx_SetTxFiFo(pcd_handle, 0, 0x40);
    HAL_PCDEx_SetTxFiFo(pcd_handle, 1, 0x80);

    setBcdUSB(DeviceDesc::USB_2_0);
    setDeviceClass(0x00);
    setDeviceSubClass(0x00);
    setDeviceProtocol(0x00);
    setEp0PacketSize(DeviceDesc::EP0PackSize64);
    setVendorID(0x0000);
    setProductID(0x0000);

    _devDesc.setByte(DeviceDesc::iManufacturer, MANUFACTURER_STR_DESC_INDEX);
    _devDesc.setByte(DeviceDesc::iProduct, PRODUCT_STR_DESC_INDEX);
    _devDesc.setByte(DeviceDesc::iSerialNumber, SERIAL_STR_DESC_INDEX);
    _devDesc.setByte(DeviceDesc::bNumConfigurations, max_num_cfg);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::~UsbDevice()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t *UsbDevice::getDeviceDescriptor(UsbDevice::Speed, uint16_t *len)
{
    *len = _devDesc.length();
    return *_devDesc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t *UsbDevice::getLangIDStrDescriptor(UsbDevice::Speed, uint16_t *len)
{
    *len = _langIDDesc.length();
    return *_langIDDesc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t *UsbDevice::getManufacturerStrDescriptor(UsbDevice::Speed, uint16_t *len)
{
    *len = _manufacturerStrDesc.length();
    return *_manufacturerStrDesc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t *UsbDevice::getProductStrDescriptor(UsbDevice::Speed, uint16_t *len)
{
    *len = _productStrDesc.length();
    return *_productStrDesc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t *UsbDevice::getSerialStrDescriptor(UsbDevice::Speed, uint16_t *len)
{
    *len = _productStrDesc.length();
    return *_productStrDesc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::setupStage(uint8_t *setup)
{
    parseSetupRequest(setup);

    _ep0_state = EP0_SETUP;
    _ep0_data_len = _request.wLength;

    switch (_request.bmRequest & 0x1F) {
    case REQ_RECIPIENT_DEVICE:
        stdDevReq(&_request);
        break;

    case REQ_RECIPIENT_INTERFACE:
        stdItfReq(&_request);
        break;

    case REQ_RECIPIENT_ENDPOINT:
        stdEPReq(&_request);
        break;

    default:
        stallEP(_request.bmRequest & 0x80);
        break;
    }
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::dataOutStage(uint8_t epnum, uint8_t *pdata)
{
    Endpoint    *pep;

    if(epnum == 0) {
    	pep = &_ep_out[0];

        if ( _ep0_state == EP0_DATA_OUT) {
            if(pep->rem_length > pep->maxpacket) {
                pep->rem_length -=  pep->maxpacket;
                ctlContinueRx (pdata, MIN(pep->rem_length ,pep->maxpacket));
            }
            else {
                if(_dev_state == DEV_CONFIGURED)
                    classEP0_RxReady();
                ctlSendStatus();
            }
        }
    }
    else if(_dev_state == DEV_CONFIGURED)
        classDataOut(epnum);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::dataInStage(uint8_t epnum, uint8_t *pdata)
{
    Endpoint    *pep;

    if(epnum == 0)
    {
    	pep = &_ep_in[0];

        if ( _ep0_state == EP0_DATA_IN) {
            if(pep->rem_length > pep->maxpacket) {
                pep->rem_length -=  pep->maxpacket;

                ctlContinueSendData (pdata, pep->rem_length);

                /* Prepare endpoint for premature end of transfer */
                prepareReceive (0, NULL, 0);
            }
            else { /* last packet is MPS multiple, so send ZLP packet */
                if((pep->total_length % pep->maxpacket == 0) &&
                        (pep->total_length >= pep->maxpacket) &&
                        (pep->total_length < _ep0_data_len )) {

                    ctlContinueSendData(NULL, 0);
                    _ep0_data_len = 0;

                    /* Prepare endpoint for premature end of transfer */
                    prepareReceive (0, NULL, 0);
                }
                else {
                    if(_dev_state == DEV_CONFIGURED)
                        classEP0_TxSent();
                    ctlReceiveStatus();
                }
            }
        }
        if (_dev_test_mode == 1) {
            runTestMode();
            _dev_test_mode = 0;
        }
    }
    else if(_dev_state == DEV_CONFIGURED)
        classDataIn(epnum);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::SOF()
{
    if(_dev_state == DEV_CONFIGURED)
        classSOF();
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::suspend()
{
    _dev_old_state =  _dev_state;
    _dev_state  = DEV_SUSPENDED;
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::resume()
{
    _dev_state = _dev_old_state;
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::isoOutIncomplete(uint8_t epnum)
{
    return classIsoOutIncomplete(epnum);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::isoInIncomplete(uint8_t epnum)
{
    return classIsoInIncomplete(epnum);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::connected()
{
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::disconnected()
{
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::setSpeed(Speed speed)
{
    _dev_speed = speed;
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::reset(Speed speed)
{
	setSpeed(speed);
    /* Open EP0 OUT */
    openEP(0x00, EP_CTRL, USB_MAX_EP0_SIZE);

    _ep_out[0].maxpacket = USB_MAX_EP0_SIZE;

    /* Open EP0 IN */
    openEP(0x80, EP_CTRL, USB_MAX_EP0_SIZE);

    _ep_in[0].maxpacket = USB_MAX_EP0_SIZE;
    /* Upon Reset call user call back */
    _dev_state = DEV_DEFAULT;

    classDeInit(_dev_config);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::parseSetupRequest(uint8_t *pdata)
{
    _request.bmRequest     = *(uint8_t *)  (pdata);
    _request.bRequest      = *(uint8_t *)  (pdata +  1);
    _request.wValue        = SWAPBYTE      (pdata +  2);
    _request.wIndex        = SWAPBYTE      (pdata +  4);
    _request.wLength       = SWAPBYTE      (pdata +  6);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::stdDevReq(UsbDevice::SetupRequest *req) {
    Status ret = OK;

    switch (req->bRequest) {
    case REQ_GET_DESCRIPTOR:
        getDescriptor (req) ;
        break;

    case REQ_SET_ADDRESS:
        setAddress(req);
        break;

    case REQ_SET_CONFIGURATION:
        setConfig (req);
        break;

    case REQ_GET_CONFIGURATION:
        getConfig (req);
        break;

    case REQ_GET_STATUS:
        getStatus (req);
        break;

    case REQ_SET_FEATURE:
        setFeature (req);
        break;

    case REQ_CLEAR_FEATURE:
        clrFeature (req);
        break;

    default:
        ctlError(req);
        break;
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::stdItfReq(UsbDevice::SetupRequest *req) {
    Status ret = OK;

    switch (_dev_state) {
    case DEV_CONFIGURED:
        if (LOBYTE(req->wIndex) <= _max_num_interfaces) {
            ret = classSetup (req);
            if((req->wLength == 0)&& (ret == OK))
                ctlSendStatus();
        }
        else ctlError(req);
        break;

    default:
        ctlError(req);
        break;
    }
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::stdEPReq(UsbDevice::SetupRequest *req) {
    uint8_t   ep_addr;
    Status ret = OK;
    Endpoint   *pep;
    ep_addr  = LOBYTE(req->wIndex);

    /* Check if it is a class request */
    if ((req->bmRequest & 0x60) == 0x20) {
        classSetup (req);
        return OK;
    }

    switch (req->bRequest) {

    case REQ_SET_FEATURE :
        switch (_dev_state) {

        case DEV_ADDRESSED:
            if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                stallEP(ep_addr);
            }
            break;

        case DEV_CONFIGURED:
            if (req->wValue == FEATURE_EP_HALT) {
                if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                    stallEP(ep_addr);
                }
            }
            classSetup (req);
            ctlSendStatus();
            break;

        default:
            ctlError(req);
            break;
        }
        break;

    case REQ_CLEAR_FEATURE :
        switch (_dev_state) {

        case DEV_ADDRESSED:
            if ((ep_addr != 0x00) && (ep_addr != 0x80)) {
                stallEP(ep_addr);
            }
            break;

        case DEV_CONFIGURED:
            if (req->wValue == FEATURE_EP_HALT) {
                if ((ep_addr & 0x7F) != 0x00) {
                    clearStallEP(ep_addr);
                    classSetup (req);
                }
                ctlSendStatus();
            }
            break;

        default:
            ctlError(req);
            break;
        }
        break;

    case REQ_GET_STATUS:
        switch (_dev_state) {

        case DEV_ADDRESSED:
            if ((ep_addr & 0x7F) != 0x00) {
                stallEP(ep_addr);
            }
            break;

        case DEV_CONFIGURED:
            pep = ((ep_addr & 0x80) == 0x80) ? &_ep_in[ep_addr & 0x7F] : &_ep_out[ep_addr & 0x7F];
            if(isStallEP(ep_addr)) {
                pep->status = 0x0001;
            }
            else {
                pep->status = 0x0000;
            }
            ctlSendData ((uint8_t *)&pep->status, 2);
            break;

        default:
            ctlError(req);
            break;
        }
        break;

    default:
        break;
    }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::getDescriptor(UsbDevice::SetupRequest *req)
{
    uint16_t len;
    uint8_t *pbuf;

    switch (req->wValue >> 8)
    {
  #if (USBD_LPM_ENABLED == 1)
    case UsbDesc::DescBOS:
      pbuf = getBOSDescriptor(_dev_speed, &len);
      break;
  #endif
    case UsbDesc::DescDevice :
      pbuf = getDeviceDescriptor(_dev_speed, &len);
      break;

    case UsbDesc::DescConfiguration :
      if(_dev_speed == USBD_SPEED_HIGH )
      {
        pbuf   = (uint8_t *)getHSConfigDescriptor(&len);
        pbuf[1] = UsbDesc::DescConfiguration;
      }
      else
      {
        pbuf   = (uint8_t *)getFSConfigDescriptor(&len);
        pbuf[1] = UsbDesc::DescConfiguration;
      }
      break;

    case UsbDesc::DescString :
        switch ((uint8_t)(req->wValue)) {
        case LANGID_STR_DESC_INDEX:
            pbuf = getLangIDStrDescriptor(_dev_speed, &len);
            break;

        case MANUFACTURER_STR_DESC_INDEX:
            pbuf = getManufacturerStrDescriptor(_dev_speed, &len);
            break;

        case PRODUCT_STR_DESC_INDEX:
            pbuf = getProductStrDescriptor(_dev_speed, &len);
            break;

        case SERIAL_STR_DESC_INDEX:
            pbuf = getSerialStrDescriptor(_dev_speed, &len);
            break;

        case CONFIG_STR_DESC_INDEX:
            pbuf = getConfigurationStrDescriptor(_dev_speed, &len);
            break;

        case INTERFACE_STR_DESC_INDEX:
            pbuf = getInterfaceStrDescriptor(_dev_speed, &len);
            break;

        default:
#if (USBD_SUPPORT_USER_STRING == 1)
            pbuf = getUsrStrDescriptor((req->wValue) , &len);
            break;
#else
            ctlError(req);
            return;
#endif
        }
        break;

    case UsbDesc::DescDeviceQualifier :
        if(_dev_speed == USBD_SPEED_HIGH  ) {
            pbuf   = (uint8_t *)getDeviceQualifierDescriptor(&len);
            break;
        }
        else {
            ctlError(req);
            return;
        }

    case UsbDesc::DescOtherSpeedConfiguration :
        if(_dev_speed == USBD_SPEED_HIGH  ) {
            pbuf   = (uint8_t *)getOtherSpeedConfigDescriptor(&len);
            pbuf[1] = UsbDesc::DescOtherSpeedConfiguration;
            break;
        }
        else {
            ctlError(req);
            return;
        }

    default:
        ctlError(req);
        return;
    }

    if((len != 0)&& (req->wLength != 0)) {
        len = MIN(len , req->wLength);
        ctlSendData(pbuf, len);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::setAddress(UsbDevice::SetupRequest *req)
{
    uint8_t  dev_addr;

    if ((req->wIndex == 0) && (req->wLength == 0)) {
        dev_addr = (uint8_t)(req->wValue) & 0x7F;

        if (_dev_state == DEV_CONFIGURED) {
            ctlError(req);
        }
        else {
            _dev_address = dev_addr;
            setUSBAddress(dev_addr);
            ctlSendStatus();

            if (dev_addr != 0) {
                _dev_state  = DEV_ADDRESSED;
            }
            else {
                _dev_state  = DEV_DEFAULT;
            }
        }
    }
    else {
        ctlError(req);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::setConfig(UsbDevice::SetupRequest *req)
{
    static uint8_t  cfgidx;

    cfgidx = (uint8_t)(req->wValue);

    if (cfgidx > _max_num_configuration ) {
        ctlError(req);
    }
    else {
        switch (_dev_state) {
        case DEV_ADDRESSED:
            if (cfgidx) {
                _dev_config = cfgidx;
                _dev_state = DEV_CONFIGURED;
                if(classInit(cfgidx) == FAIL) {
                    ctlError(req);
                    return;
                }
                ctlSendStatus();
            }
            else {
                ctlSendStatus();
            }
            break;

        case DEV_CONFIGURED:
            if (cfgidx == 0) {
                _dev_state = DEV_ADDRESSED;
                _dev_config = cfgidx;
                classDeInit(cfgidx);
                ctlSendStatus();
            }
            else  if (cfgidx != _dev_config) {
                /* Clear old configuration */
                classDeInit(_dev_config);

                /* set new configuration */
                _dev_config = cfgidx;
                if(classInit(cfgidx) == FAIL) {
                    ctlError(req);
                    return;
                }
                ctlSendStatus();
            }
            else {
                ctlSendStatus();
            }
            break;

        default:
            ctlError(req);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::getConfig(UsbDevice::SetupRequest *req)
{
	static const uint8_t DEFAULT_CONFIG = 0;

    if (req->wLength != 1) {
        ctlError(req);
    }
    else {
        switch (_dev_state ) {
        case DEV_ADDRESSED:
            ctlSendData ((uint8_t *)&DEFAULT_CONFIG, 1);
            break;

        case DEV_CONFIGURED:
            ctlSendData ((uint8_t *)&_dev_config, 1);
            break;

        default:
            ctlError(req);
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::getStatus(UsbDevice::SetupRequest *req)
{
    switch (_dev_state) {
    case DEV_ADDRESSED:
    case DEV_CONFIGURED:

  #if ( USBD_SELF_POWERED == 1)
        _dev_config_status = CONFIG_SELF_POWERED;
  #else
        _dev_config_status = 0;
  #endif

        if (_dev_remote_wakeup) {
            _dev_config_status |= CONFIG_REMOTE_WAKEUP;
        }

        ctlSendData ((uint8_t *)&_dev_config_status, 2);
        break;

    default :
        ctlError(req);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::setFeature(UsbDevice::SetupRequest *req)
{
    if (req->wValue == FEATURE_REMOTE_WAKEUP) {
        _dev_remote_wakeup = 1;
        classSetup (req);
        ctlSendStatus();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::clrFeature(UsbDevice::SetupRequest *req)
{
    switch (_dev_state) {
    case DEV_ADDRESSED:
    case DEV_CONFIGURED:
        if (req->wValue == FEATURE_REMOTE_WAKEUP) {
            _dev_remote_wakeup = 0;
            classSetup (req);
            ctlSendStatus();
        }
        break;

    default :
        ctlError(req);
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::ctlError(UsbDevice::SetupRequest *req)
{
    (void)req;
    stallEP(0x80);
    stallEP(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlSendData(uint8_t *pbuf, uint16_t len)
{
    /* Set EP0 State */
    _ep0_state          	= EP0_DATA_IN;
    _ep_in[0].total_length 	= len;
    _ep_in[0].rem_length   	= len;
   /* Start the transfer */
    transmit (0x00, pbuf, len);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::transmit(uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
    HAL_PCD_EP_Transmit(_pcd_handle, ep_addr, pbuf, size);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::setUSBAddress(uint8_t dev_addr)
{
    HAL_PCD_SetAddress(_pcd_handle, dev_addr);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlSendStatus()
{
    /* Set EP0 State */
    _ep0_state = EP0_STATUS_IN;

   /* Start the transfer */
    transmit (0x00, NULL, 0);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlContinueRx(uint8_t *pbuf, uint16_t len)
{
    prepareReceive (0, pbuf, len);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::prepareReceive(uint8_t ep_addr, uint8_t *pbuf, uint16_t size)
{
    HAL_PCD_EP_Receive(_pcd_handle, ep_addr, pbuf, size);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlContinueSendData(uint8_t *pbuf, uint16_t len) {
    /* Start the next transfer */
    transmit (0x00, pbuf, len);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlPrepareRx(uint8_t * pbuf, uint16_t len) {
    /* Set EP0 State */
    _ep0_state = EP0_DATA_OUT;
    _ep_out[0].total_length = len;
    _ep_out[0].rem_length   = len;
    /* Start the transfer */
    prepareReceive(0, pbuf, len);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::ctlReceiveStatus()
{
    /* Set EP0 State */
    _ep0_state = EP0_STATUS_OUT;
   /* Start the transfer */
    prepareReceive ( 0, NULL, 0);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::runTestMode()
{
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::openEP(uint8_t ep_addr, EPType ep_type, uint16_t ep_mps)
{
    HAL_PCD_EP_Open(_pcd_handle, ep_addr, ep_mps, ep_type);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::closeEP(uint8_t ep_addr)
{
    HAL_PCD_EP_Close(_pcd_handle, ep_addr);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::flushEP(uint8_t ep_addr)
{
    HAL_PCD_EP_Flush(_pcd_handle, ep_addr);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::stallEP(uint8_t epnum)
{
    HAL_PCD_EP_SetStall(_pcd_handle, epnum);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

UsbDevice::Status UsbDevice::clearStallEP(uint8_t epnum)
{
    HAL_PCD_EP_ClrStall(_pcd_handle, epnum);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint8_t UsbDevice::isStallEP(uint8_t epnum)
{
    if((epnum & 0x80) == 0x80)
    {
      return _pcd_handle->IN_ep[epnum & 0x7F].is_stall;
    }
    else
    {
      return _pcd_handle->OUT_ep[epnum & 0x7F].is_stall;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t UsbDevice::getRxDataSize(uint8_t ep_addr)
{
    return HAL_PCD_EP_GetRxCount(_pcd_handle, ep_addr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void UsbDevice::delay(uint32_t value)
{
    HAL_Delay(value);
}

#endif
