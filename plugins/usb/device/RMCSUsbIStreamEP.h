/*
 * RMCSUsbIStreamEP.h
 *
 *  Created on: Dec 18, 2017
 *      Author: ivan
 */

#ifndef DEVICE_RMCSUSBISTREAMEP_H_
#define DEVICE_RMCSUSBISTREAMEP_H_
#include <core/device/istreamnode.h>
#include <plugins/usb/device/XUsbDevice.h>

class RMCSUsbIface;

///////////////////////////////////////////////////////////////////////////////////////

class RMCSUsbIStreamEP :
		public IStreamNode,
		public XUsbOutEndpoint
{
public:
	RMCSUsbIStreamEP(const XUsbEndpoint & source,
					 const char * name,
					 Length_t num_packets,
					 uint8_t epnum,
					 uint16_t mps);

protected:
	virtual bool settingsRequested(ControlPacket & packet) const final override;

	virtual void streamToggled(bool enabled) final override;

	virtual bool epDataOut(uint8_t * pdata) final override;

private:
    uint8_t    _inPacketCounter;
};

#endif /* DEVICE_RMCSUSBISTREAMEP_H_ */
