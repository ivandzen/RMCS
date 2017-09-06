/*
 * XUsbDevice.h
 *
 *  Created on: Sep 3, 2017
 *      Author: ivan
 */

#ifndef XUSBDEVICE_H_
#define XUSBDEVICE_H_
#include <plugins/usb/common/usbdescriptors.h>
#include "XUsbDevice_Config.h"

/////////////////////////////////////////////////////////////////////////////////////////

class UsbZeroEndpoint;

class UsbEndpoint :
		public UsbEPDescriptor
{
public:
	UsbEndpoint(void * handle,
				const UsbEPDescriptor & other) :
		UsbEPDescriptor(other),
		_handle(handle)
	{}

	virtual ~UsbEndpoint() {}

	virtual bool setupRequest(UsbSetupRequest * req) = 0;

	inline void open() { HAL_XUsbDevice_OpenEP(_handle, bEndpointAddress(), wMaxPacketSize(), bmAttributes() & UsbEPTypeMask); }

	inline void close() { HAL_XUsbDevice_CloseEP(_handle, bEndpointAddress()); }

	inline void flush() { HAL_XUsbDevice_FlushEP(_handle, bEndpointAddress()); }

	inline void stall()
	{
		HAL_XUsbDevice_StallEP(_handle, bEndpointAddress());
		_status = 0x0001;
		stallEvent();
	}

	inline void clearStall()
	{
		HAL_XUsbDevice_ClearStallEP(_handle, bEndpointAddress());
		_status = 0x0001;
		clearStallEvent();
	}

	void reportStatus(UsbZeroEndpoint * ep);

	uint16_t getRxCount() const { return HAL_XUsbDevice_GetRxCount(_handle, bEndpointAddress()); }

	inline void * handle() const { return _handle; }

protected:
	virtual void stallEvent() {}

	virtual void clearStallEvent() {}

private:
	void * 		_handle;
	uint16_t	_status;
};

/////////////////////////////////////////////////////////////////////////////////////////

class UsbInEndpoint :
		public UsbEndpoint
{
public:
	UsbInEndpoint(void * handle,
				  const UsbEPDescriptor & other) :
		UsbEndpoint(handle, other)
	{}

	virtual bool epDataIn(uint8_t * pdata) = 0;

	inline void transmit(uint8_t * pbuf, uint16_t size)
	{
		HAL_XUsbDevice_Transmit(handle(), bEndpointAddress(), pbuf, size);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

class UsbOutEndpoint :
		public UsbEndpoint
{
public:
	UsbOutEndpoint(void * handle,
					const UsbEPDescriptor & other) :
		UsbEndpoint(handle, other)
	{}

	virtual bool epDataOut(uint8_t * pdata) = 0;

	inline void receive(uint8_t * pbuf, uint16_t size)
	{
		HAL_XUsbDevice_Receive(handle(), bEndpointAddress(), pbuf, size);
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

class UsbZeroEndpoint :
		public UsbInEndpoint,
		public UsbOutEndpoint
{
public:
	UsbZeroEndpoint(void * handle,
					uint8_t max_packet) :
		UsbInEndpoint(handle, UsbEPDescriptor()),
		UsbOutEndpoint(handle, UsbEPDescriptor()),
	    _state(EP0_IDLE),
	    _inTotalLength(0),
	    _inRemLength(0),
	    _outTotalLength(0),
	    _outRemLength(0),
	    _maxPacket(max_packet),
	    _dataLength(0)
	{}

	inline uint8_t ctlMaxPacket() const { return _maxPacket; }

	inline void ctlTransmit(uint8_t * pdata, uint16_t len)
	{
		/* Set EP0 State */
		_state      	= EP0_DATA_IN;
		_inTotalLength 	= len;
		_inRemLength	= len;
		/* Start the transfer */
		UsbInEndpoint::transmit(pdata, len);
	}

	inline void ctlReceive(uint8_t * pdata, uint16_t len)
	{
		/* Set EP0 State */
		_state = EP0_DATA_OUT;
		_outTotalLength = len;
		_outRemLength   = len;
		/* Start the transfer */
		UsbOutEndpoint::receive(pdata, len);
	}

	inline void ctlSendStatus()
	{
		/* Set EP0 State */
		_state = EP0_STATUS_IN;

		/* Start the transfer */
		UsbInEndpoint::transmit(nullptr, 0);
	}

	inline void ctlReceiveStatus()
	{
	    /* Set EP0 State */
	    _state = EP0_STATUS_OUT;
	   /* Start the transfer */
	    UsbOutEndpoint::receive (nullptr, 0);
	}

	bool setupStage(uint8_t * pdata);

	inline void ctlError()
	{
		UsbInEndpoint::stall();
		UsbOutEndpoint::stall();
	}

protected:
	virtual bool isDeviceConfigured() const = 0;

	virtual void stdDevReq(UsbSetupRequest * req) = 0;

	virtual void stdItfReq(UsbSetupRequest * req) = 0;

	virtual void stdEPReq(UsbSetupRequest * req) = 0;

	virtual void ep0TxSent() {}

	virtual void ep0RxReady() {}

	virtual bool epDataOut(uint8_t * pdata) final override;

	virtual bool epDataIn(uint8_t * pdata) final override;

private:
    typedef enum
	{
        EP0_IDLE,
        EP0_SETUP,
        EP0_DATA_IN,
        EP0_DATA_OUT,
        EP0_STATUS_IN,
        EP0_STATUS_OUT,
        EP0_STALL
    }
    EP0State;

    EP0State 		_state;
    uint32_t		_inTotalLength;
    uint32_t		_inRemLength;
    uint32_t		_outTotalLength;
    uint32_t		_outRemLength;
    uint32_t		_maxPacket;
    uint16_t		_dataLength;
    UsbSetupRequest _request;
};

/////////////////////////////////////////////////////////////////////////////////////////

class UsbConfiguration :
		public UsbConfigDescriptor
{
public:
	virtual ~UsbConfiguration() {}

	virtual bool init(UsbInEndpoint * inEndpoints,
					  UsbOutEndpoint * outEndpoints) = 0;

	virtual void deInit() = 0;

	virtual bool setupRequest(UsbSetupRequest * req) = 0;
};

/////////////////////////////////////////////////////////////////////////////////////////

class XUsbDevice :
	public UsbZeroEndpoint
{
public:

    typedef enum
	{
        OK,
        BUSY,
        FAIL
    }
    Status;

    typedef enum
	{
        USBD_SPEED_HIGH  = 0,
        USBD_SPEED_FULL  = 1,
        USBD_SPEED_LOW   = 2,
    }
    Speed;

	XUsbDevice();
	virtual ~XUsbDevice();

    bool  	dataOutStage(uint8_t epnum, uint8_t * pdata);
    bool	dataInStage(uint8_t epnum, uint8_t * pdata);
    Status  SOF();
    Status  reset(Speed speed);
    Status  suspend();
    Status  resume();
    Status  isoOutIncomplete(uint8_t epnum);
    Status  isoInIncomplete(uint8_t epnum);
    virtual void  connectEvent();
    virtual void  disconnectEvent();

protected:
	bool setString(uint8_t idx,
				   const UsbStringDescriptor & string);

	bool setConfig(uint8_t idx,
				   UsbConfiguration * config);

	virtual void runTestMode() {}

	virtual void SOFEvent() {}

	virtual void suspendEvent() {}

	virtual void resumeEvent() {}

	virtual void isoOutIncompleteEvent(uint8_t epnum) {}

	virtual void isoInIncompleteEvent(uint8_t epnum) {}

	virtual void resetEvent() {}

private:
    virtual void stdDevReq(UsbSetupRequest * req) final override;
    virtual void stdItfReq(UsbSetupRequest * req) final override;
    virtual void stdEPReq(UsbSetupRequest * req) final override;

    void 	setAddress(UsbSetupRequest * req);
    void	setConfig(UsbSetupRequest * req);
    void 	getConfig(UsbSetupRequest * req);
    void	getStatus(UsbSetupRequest * req);
    void	setFeature(UsbSetupRequest * req);
    void	clrFeature(UsbSetupRequest * req);
    void 	getDescriptor(UsbSetupRequest * req);

    enum DeviceState
	{
        DEV_DEFAULT,
        DEV_ADDRESSED,
        DEV_CONFIGURED,
        DEV_SUSPENDED
    };

    enum DevConfig
	{
        CONFIG_REMOTE_WAKEUP    = 2,
        CONFIG_SELF_POWERED     = 1
    };

    int					_hsConfIdx;
    int					_defConfIdx;
    void *				_handle;
    bool				_dev_test_mode;
    Speed               _dev_speed;
    DeviceState         _dev_old_state;
    DeviceState         _dev_state;
    uint8_t				_dev_address;
    uint32_t            _dev_config_status;
    uint32_t            _dev_remote_wakeup;
    uint8_t				_dev_config;

    UsbDeviceDescriptor	_devDescriptor;
    UsbStringDescriptor	_strings[USB_MAX_STRINGS];

    UsbConfiguration *	_configs[USB_MAX_CONFIGS];
    UsbInEndpoint *		_inEndpoints[USB_MAX_ENDPOINTS];
    UsbOutEndpoint *	_outEndpoints[USB_MAX_ENDPOINTS];
};

#endif /* XUSBDEVICE_H_ */
