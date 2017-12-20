/*
 * RMCSUsbOStreamEP.h
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#ifndef DEVICE_RMCSUSBOSTREAMEP_H_
#define DEVICE_RMCSUSBOSTREAMEP_H_
#include <core/device/ostreamnode.h>
#include <plugins/usb/device/XUsbDevice.h>

///////////////////////////////////////////////////////////////////////////////////////

class RMCSUsbOStreamEP :
		public OStreamNode,
		public XUsbInEndpoint
{
public:
	RMCSUsbOStreamEP(const XUsbEndpoint & source,
					 const char * name,
					 uint8_t epnum,
					 uint16_t mps);

protected:
	virtual bool settingsRequested(ControlPacket & packet) const final override;

	virtual void streamToggled(bool enabled) final override;

protected:
	virtual bool epDataIn(uint8_t * pdata) final override;
};

#endif /* DEVICE_RMCSUSBOSTREAMEP_H_ */
