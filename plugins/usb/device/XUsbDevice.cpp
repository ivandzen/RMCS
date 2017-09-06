/*
 * XUsbDevice.cpp
 *
 *  Created on: Sep 3, 2017
 *      Author: ivan
 */

#include <plugins/usb/device/XUsbDevice.h>

#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define  SWAPBYTE(addr)        (((uint16_t)(*((uint8_t *)(addr)))) + \
                               (((uint16_t)(*(((uint8_t *)(addr)) + 1))) << 8))

/////////////////////////////////////////////////////////////////////////////////////////

void UsbEndpoint::reportStatus(UsbZeroEndpoint * ep0)
{
	ep0->transmit(reinterpret_cast<uint8_t*>(&_status), 2);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool UsbZeroEndpoint::epDataOut(uint8_t * pdata)
{
	if ( _state == EP0_DATA_OUT)
	{
		if(_outRemLength > _maxPacket)
		{
			_outRemLength -=  _maxPacket;
			UsbOutEndpoint::receive(pdata, MIN(_outRemLength ,_maxPacket));
		}
		else
		{
			if(isDeviceConfigured())
				ep0RxReady();
			ctlSendStatus();
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool UsbZeroEndpoint::epDataIn(uint8_t * pdata)
{
	if (_state == EP0_DATA_IN)
	{
		if(_inRemLength > _maxPacket)
		{
			_inRemLength -=  _maxPacket;
			UsbInEndpoint::transmit(pdata, _inRemLength);

			/* Prepare endpoint for premature end of transfer */
			UsbOutEndpoint::receive(nullptr, 0);
		}
		else
		{ /* last packet is MPS multiple, so send ZLP packet */
			if((_inTotalLength % _maxPacket == 0) &&
				(_inTotalLength >= _maxPacket) &&
				(_inTotalLength < _dataLength ))
			{
				UsbInEndpoint::transmit(nullptr, 0);
				_dataLength = 0;

				/* Prepare endpoint for premature end of transfer */
				UsbOutEndpoint::receive(nullptr, 0);
			}
			else
			{
				if(isDeviceConfigured())
					ep0TxSent();
				ctlReceiveStatus();
			}
		}
	}

	if (_dev_test_mode == 1)
	{
		runTestMode();
		_dev_test_mode = 0;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool UsbZeroEndpoint::setupStage(uint8_t * pdata)
{
    _request.bmRequest     = *(uint8_t *)  (pdata);
    _request.bRequest      = *(uint8_t *)  (pdata +  1);
    _request.wValue        = SWAPBYTE      (pdata +  2);
    _request.wIndex        = SWAPBYTE      (pdata +  4);
    _request.wLength       = SWAPBYTE      (pdata +  6);

    _state = EP0_SETUP;
    _dataLength = _request.wLength;

    switch (_request.bmRequest & 0x1F)
    {
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
    {
    	if(_request.bmRequest & 0x80)
    		UsbInEndpoint::stall();
    	else
    		UsbOutEndpoint::stall();
        break;
    }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

XUsbDevice::XUsbDevice()
{

}

/////////////////////////////////////////////////////////////////////////////////////////

XUsbDevice::~XUsbDevice()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

bool XUsbDevice::setString(uint8_t idx, const UsbStringDescriptor & string)
{
	if(idx >= USB_MAX_STRINGS)
		return false;
	_strings[idx] = string;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool  XUsbDevice::setConfig(uint8_t idx, UsbConfiguration * config)
{
	if((idx == 0) || (idx > USB_MAX_CONFIGS) || (_configs[idx] != nullptr))
		return false;
	_configs[idx] = config;
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool  XUsbDevice::dataOutStage(uint8_t epnum, uint8_t * pdata)
{
	if(epnum == 0)
		return _outEndpoints[epnum]->epDataOut(pdata);
	else if(_dev_state == DEV_CONFIGURED)
		return _outEndpoints[epnum]->epDataOut(pdata);

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool  XUsbDevice::dataInStage(uint8_t epnum, uint8_t * pdata)
{
	if(epnum == 0)
		return _inEndpoints[epnum]->epDataIn(pdata);
	else if(_dev_state == DEV_CONFIGURED)
		return _inEndpoints[epnum]->epDataIn(pdata);

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::SOF()
{
	if(_dev_state == DEV_CONFIGURED)
		SOFEvent();
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::suspend()
{
	_dev_old_state =  _dev_state;
	_dev_state  = DEV_SUSPENDED;
	suspendEvent();
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::resume()
{
	_dev_state = _dev_old_state;
	resumeEvent();
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::isoOutIncomplete(uint8_t epnum)
{
	if(_dev_state == DEV_CONFIGURED)
		isoOutIncompleteEvent(epnum);
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::isoInIncomplete(uint8_t epnum)
{
	if(_dev_state == DEV_CONFIGURED)
		isoInIncompleteEvent(epnum);
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::reset(Speed speed)
{
	//! @attention !!!
	_dev_speed = speed;
    /* Open EP0 OUT */
	_inEndpoints[0]->open();

    /* Open EP0 IN */
	_outEndpoints[0]->open();

    /* Upon Reset call user call back */
    _dev_state = DEV_DEFAULT;

    _configs[_dev_config]->deInit();
    resetEvent();
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::stdDevReq(UsbSetupRequest * req)
{
    switch (req->bRequest) {
    case REQ_GET_DESCRIPTOR:
        getDescriptor (req);
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
        ctlError();
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::stdItfReq(UsbSetupRequest * req)
{
	static uint8_t ifalt = 0;
    bool ret = true;

    switch (_dev_state)
    {
    case DEV_CONFIGURED:
    {
        if (LOBYTE(req->wIndex) <= _configs[_dev_config]->bNumInterfaces())
        {
        	switch (req->bmRequest & REQ_TYPE_MASK)
        	{
        	case REQ_TYPE_CLASS :
        	{
        		ret = _configs[_dev_config]->setupRequest(req);
        		break;
        	}

        	case REQ_TYPE_STANDARD:
        	{
        		switch (req->bRequest)
        		{
        		case REQ_GET_INTERFACE :
        			ctlTransmit(&ifalt, 1);
        			break;

        		case REQ_SET_INTERFACE :
        		{
        			break;
        		}
        		}
        	}
        	}

            if((req->wLength == 0) && ret)
                ctlSendStatus();
        }
        else
        	ctlError();
        break;
    }

    default:
        ctlError();
        break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void  XUsbDevice::stdEPReq(UsbSetupRequest * req)
{
    uint8_t ep_addr  = LOBYTE(req->wIndex);

    UsbEndpoint * ep;
    if(ep_addr & 0x80)
    	ep = _inEndpoints[ep_addr & 0x7F];
    else
    	ep = _outEndpoints[ep_addr & 0x7F];

    if ((req->bmRequest & 0x60) == 0x20)
    {
    	ep->setupRequest(req);
    	return;
    }

    switch(_dev_state)
    {
    case DEV_ADDRESSED :
    {
    	if ((ep_addr != 0x00) && (ep_addr != 0x80))
    		ep->stall();
    	return;
    }

    case DEV_CONFIGURED :
    {
    	switch (req->bRequest)
    	{
    	case REQ_SET_FEATURE :
    	{
    		if ((req->wValue == UsbFeature_EP_HALT) &&
    			(ep_addr != 0x00) && (ep_addr != 0x80))
    				ep->stall();

    		ctlSendStatus();
    		break;
    	}

    	case REQ_CLEAR_FEATURE :
    	{
    		if ((req->wValue == UsbFeature_EP_HALT) &&
    			((ep_addr & 0x7F) != 0x00))
    			ep->clearStall();

    		ctlSendStatus();
    		break;
    	}

    	case REQ_GET_STATUS:
    		ep->reportStatus(this);
    		break;

    	}
    	break;
    }

    default :
    	ctlError();
    	break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::getDescriptor(UsbSetupRequest *req)
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
    case UsbDescType_Device:
    {
    	pbuf = _devDescriptor.data();
    	len = _devDescriptor.bLength();
    	break;
    }

    case UsbDescType_Configuration :
    {
    	UsbConfiguration * config;

    	if(_dev_speed == USBD_SPEED_HIGH )
    		config = _configs[_hsConfIdx];
    	else
    		config = _configs[_defConfIdx];

    	pbuf = (uint8_t *)(config->data());
    	len = config->wTotalLength();
        break;

    }

    case UsbDescType_String :
    {
    	uint8_t str_idx = (uint8_t)(req->wValue);
    	if((str_idx < USB_MAX_STRINGS) && (_strings[str_idx].isValid()))
    		pbuf = _strings[str_idx].data();
    	else
    		ctlError();
    	break;
    }

    case UsbDescType_DeviceQualifier :
        if(_dev_speed == USBD_SPEED_HIGH  )
        {
        	//! @attention
        	pbuf = nullptr;
        	len = 0;
            break;
        }
        else {
            ctlError();
            return;
        }

    case UsbDescType_OtherSpeedConfiguration :
        if(_dev_speed == USBD_SPEED_HIGH  )
        {
            pbuf   = (uint8_t *)getOtherSpeedConfigDescriptor(&len);
            pbuf[1] = UsbDesc::DescOtherSpeedConfiguration;
            break;
        }
        else {
            ctlError();
            return;
        }

    default:
        ctlError();
        return;
    }

    if((len != 0)&& (req->wLength != 0))
    {
        len = MIN(len , req->wLength);
        ctlTransmit(pbuf, len);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::setAddress(UsbSetupRequest *req)
{
    uint8_t  dev_addr;

    if ((req->wIndex == 0) && (req->wLength == 0))
    {
        dev_addr = (uint8_t)(req->wValue) & 0x7F;

        if (_dev_state == DEV_CONFIGURED)
        	ctlError();
        else
        {
            _dev_address = dev_addr;
            HAL_XUsbDevice_SetAddress(_handle, dev_addr);
            ctlSendStatus();

            if (dev_addr != 0)
                _dev_state  = DEV_ADDRESSED;
            else
                _dev_state  = DEV_DEFAULT;
        }
    }
    else
    	ctlError();
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::setConfig(UsbSetupRequest *req)
{
    static uint8_t  cfgidx = (uint8_t)(req->wValue);

    if (cfgidx > USB_MAX_CONFIGS ||
    	_configs[cfgidx] == nullptr)
    {
        ctlError();
        return;
    }

    switch (_dev_state)
    {
    case DEV_ADDRESSED:
    {
    	if (cfgidx)
    	{
    		_dev_config = cfgidx;
    		_dev_state = DEV_CONFIGURED;
    		if(!_configs[cfgidx]->init((UsbInEndpoint*)_inEndpoints,
    								   (UsbOutEndpoint*)_outEndpoints))
    		{
    			ctlError();
    			return;
    		}
    		ctlSendStatus();
    	}
    	else
    		ctlSendStatus();
    	break;
    }

    case DEV_CONFIGURED:
    {
    	if (cfgidx == 0)
    	{
    		_dev_state = DEV_ADDRESSED;
    		_dev_config = cfgidx;
    		_configs[cfgidx]->deInit();
    		ctlSendStatus();
    	}
    	else  if (cfgidx != _dev_config)
    	{
    		/* Clear old configuration */
    		_configs[_dev_config]->deInit();

    		/* set new configuration */
    		_dev_config = cfgidx;
    		if(!_configs[cfgidx]->init((UsbInEndpoint*)_inEndpoints,
    								   (UsbOutEndpoint*)_outEndpoints))
    		{
    			ctlError();
    			return;
    		}
    		ctlSendStatus();
    	}
    	else
    		ctlSendStatus();

    	break;
    }

    default:
    	ctlError();
    	break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::getConfig(UsbSetupRequest *req)
{
	static const uint8_t DEFAULT_CONFIG = 0;

    if (req->wLength != 1)
        ctlError();
    else
    {
        switch (_dev_state )
        {
        case DEV_ADDRESSED:
            ctlTransmit ((uint8_t *)&DEFAULT_CONFIG, 1);
            break;

        case DEV_CONFIGURED:
            ctlTransmit ((uint8_t *)&_dev_config, 1);
            break;

        default:
            ctlError();
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::getStatus(UsbSetupRequest *req)
{
    switch (_dev_state) {
    case DEV_ADDRESSED:
    case DEV_CONFIGURED:

  #if ( USBD_SELF_POWERED == 1)
        _dev_config_status = CONFIG_SELF_POWERED;
  #else
        _dev_config_status = 0;
  #endif

        if (_dev_remote_wakeup)
            _dev_config_status |= CONFIG_REMOTE_WAKEUP;

        ctlTransmit ((uint8_t *)&_dev_config_status, 2);
        break;

    default :
        ctlError();
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::setFeature(UsbSetupRequest * req)
{
    if (req->wValue == UsbFeature_REMOTE_WAKEUP) {
        _dev_remote_wakeup = 1;
        _configs[_dev_config]->setupRequest(req);
        ctlSendStatus();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void XUsbDevice::clrFeature(UsbSetupRequest * req)
{
    switch (_dev_state) {
    case DEV_ADDRESSED:
    case DEV_CONFIGURED:
    {
        if (req->wValue == UsbFeature_REMOTE_WAKEUP)
        {
            _dev_remote_wakeup = 0;
            _configs[_dev_config]->setupRequest(req);
            ctlSendStatus();
        }
        break;
    }

    default :
        ctlError();
        break;
    }
}

