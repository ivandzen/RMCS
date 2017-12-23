/*
 * RMCSUsbInterface.h
 *
 *  Created on: Dec 16, 2017
 *      Author: ivan
 */

#ifndef DEVICE_RMCSUSBIFACE_H_
#define DEVICE_RMCSUSBIFACE_H_

#include <plugins/usb/device/XUsbDevice.h>
#include <core/device/device.h>

#define MAX_SETUP_DATA_LENGTH 4096

class RMCSUsbIStreamEP;
class RMCSUsbOStreamEP;

class RMCSUsbIface :
		public Device,
		public XUsbIface
{
public:
	explicit RMCSUsbIface(const UsbInterfaceDescriptor & desc);

	virtual ~RMCSUsbIface();

	bool init(uint8_t number,
			  uint8_t altSetting,
			  const UsbStringDescriptor & ifaceStr);

protected:
	virtual bool setupRequest(UsbSetupRequest * req) override;

	virtual void ep0RxReady(UsbSetupRequest * req) final override;

	virtual void ep0TxSent(UsbSetupRequest * req) final override;

private:
	 UsbSetupRequest * 	_setupRequest;
	 uint8_t       		_setupDataBuffer[MAX_SETUP_DATA_LENGTH];
};

#endif /* DEVICE_RMCSUSBIFACE_H_ */
