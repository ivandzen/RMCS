/*
 * RMCSUsbOStreamEP.h
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#ifndef DEVICE_RMCSUSBOSTREAMEP_H_
#define DEVICE_RMCSUSBOSTREAMEP_H_
#include <XUsbDevice.h>
#include <core/device/ostreamnode.h>

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

	virtual void sync() final override;

protected:
	//virtual bool epDataIn(uint8_t * pdata) final override {}
	virtual bool epDataIn(uint8_t * pdata) final override;
};

#endif /* DEVICE_RMCSUSBOSTREAMEP_H_ */
