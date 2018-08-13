/*
 * usbaudiodevice.hpp
 *
 *  Created on: Aug 11, 2018
 *      Author: ivan
 */

#ifndef DEVICE_USBAUDIODEVICE_HPP_
#define DEVICE_USBAUDIODEVICE_HPP_

#include <plugins/usb/device/XUsbDevice.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSACDescriptorBase : public UsbClassSpecificDescriptor
{
public:
	enum ACSubtype_t
	{
		AC_DESCRIPTOR_UNDEFINED = 0x00,
		AC_HEADER				= 0x01,
		AC_INPUT_TERMINAL		= 0x02,
		AC_OUTPUT_TERMINAL		= 0x03,
		AC_MIXER_UNIT			= 0x04,
		AC_SELECTOR_UNIT		= 0x05,
		AC_FEATURE_UNIT			= 0x06,
		AC_EFFECT_UNIT			= 0x07,
		AC_PROCESSING_UNIT		= 0x08,
		AC_EXTENSION_UNIT		= 0x09,
		AC_CLOCK_SOURCE			= 0x0A,
		AC_CLOCK_SELECTOR		= 0x0B,
		AC_CLOCK_MULTIPLIER		= 0x0C,
		AC_SAMPLE_RATE_CONVERTER= 0x0D,
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TerminalDescriptorBase : public CSACDescriptorBase
{
protected:
	typedef struct __packed
	{
		uint8_t bDescriptorSubType;
		uint8_t bTerminalID;
		uint16_t wTerminalType;
		uint8_t bAssocTerminal;
	}
	TerminalDescriptorBaseFields;

	static const int SIZE = 2 + sizeof(TerminalDescriptorBase);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InputTerminalDescriptor : public TerminalDescriptorBase
{
private:
	typedef struct __packed
	{
		uint8_t bCSourceID;
		uint8_t bNrChannels;
		uint32_t bmChannelConfig;
		uint8_t iChannelNames;
		uint16_t bmControls;
		uint8_t iTerminal;
	}
	InputTerminalDescriptorFields;

	static const int SIZE = TerminalDescriptorBase::SIZE + sizeof(InputTerminalDescriptorFields);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OutputTerminalDescriptor : public TerminalDescriptorBase
{
private:
	typedef struct __packed
	{
		uint8_t bSourceID;
		uint8_t bCSourceID;
		uint16_t bmControls;
		uint8_t iTerminal;
	}
	OutputTerminalDescriptorFields;

	static const int SIZE = TerminalDescriptorBase::SIZE + sizeof(OutputTerminalDescriptorFields);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AudioControlIfaceDescriptor
		: public CSACDescriptorBase
{
public:
	enum AFCategory
	{
		AFC_FUNCTION_SUBCLASS_UNDEFINED = 0x00,
		AFC_DESKTOP_SPEAKER = 0x01,
		AFC_HOME_THEATER = 0x02,
		AFC_MICROPHONE = 0x03,
		AFC_HEADSET = 0x04,
		AFC_TELEPHONE = 0x05,
		AFC_CONVERTER = 0x06,
		AFC_VOICESOUND_RECORDER = 0x07,
		AFC_IO_BOX = 0x08,
		AFC_MUSICAL_INSTRUMENT = 0x09,
		AFC_PRO_AUDIO = 0x0A,
		AFC_AUDIOVIDEO = 0x0B,
		AFC_CONTROL_PANEL = 0x0C,
		AFC_OTHER = 0xFF
	};

	bool init(AFCategory category)
	{
		if(!CSACDescriptorBase::init(SIZE, CS_INTERFACE))
			return false;

		fields()->bDescriptorSubtype = AC_HEADER;
		fields()->bcdADC = 0x0001;
		fields()->bCategory = uint8_t(category);
		fields()->wTotalLength = 0x0000;
		fields()->bmControls = 0x00;
		return true;
	}

	inline InputTerminalDescriptor createInputTerminal() const;

	inline OutputTerminalDescriptor createOutputTerminal() const;

private:
	typedef struct __packed
	{
		uint8_t bDescriptorSubtype;
		uint16_t bcdADC;
		uint8_t bCategory;
		uint16_t wTotalLength;
		uint8_t bmControls;
	}
	CSACIfaceDescriptorFields;

	static const int SIZE = 2 + sizeof(CSACIfaceDescriptorFields);

	inline CSACIfaceDescriptorFields * fields() const
	{
		return reinterpret_cast<CSACIfaceDescriptorFields*>(restFields());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OutAudioStreamingEP
		: public XUsbOutEndpoint
{
public:
	inline bool init(uint8_t epnum,
					 uint8_t attributes,
					 uint16_t maxPacketSize,
					 uint8_t interval,
					 uint8_t refresh,
					 uint8_t syncAddr)
		{
			if(!XUsbOutEndpoint::init(SIZE, epnum | 0x80, attributes,
									  maxPacketSize, interval))
				return false;

			restFields()->bRefresh = refresh;
			restFields()->bSyncAddress = syncAddr;
			return true;
		}

private:
	typedef struct __packed
	{
		uint8_t bRefresh;
		uint8_t bSyncAddress;
	}
	OutASEPFields;

	static const int SIZE = DEFAULT_LENGTH + sizeof(OutASEPFields);

	inline OutASEPFields * restFields() const
	{
		return reinterpret_cast<OutASEPFields*>(XUsbOutEndpoint::restFields());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class InAudioStreamingEP : public XUsbInEndpoint
{
public:
	inline bool init(uint8_t epnum,
					 uint8_t attributes,
					 uint16_t maxPacketSize,
					 uint8_t interval,
					 uint8_t refresh,
					 uint8_t syncAddr)
		{
			if(!XUsbInEndpoint::init(SIZE, epnum | 0x80, attributes,
									 maxPacketSize, interval))
				return false;

			restFields()->bRefresh = refresh;
			restFields()->bSyncAddress = syncAddr;
			return true;
		}

private:
	typedef struct __packed
	{
		uint8_t bRefresh;
		uint8_t bSyncAddress;
	}
	InASEPFields;

	static const int SIZE = DEFAULT_LENGTH + sizeof(InASEPFields);

	inline InASEPFields * restFields() const
	{
		return reinterpret_cast<InASEPFields*>(XUsbInEndpoint::restFields());
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UsbAudioIface : public XUsbIface
{
public:
	enum AudioIfaceSubClass_t
	{
		IFACE_SUBCLASS_UNDEF = 0x00,
		IFACE_SUBCLASS_AUDIOCTL = 0x01,
		IFACE_SUBCLASS_AUDIOSTR = 0x02,
		IFACE_SUBCLASS_MIDISTR = 0x03
	};

	typedef enum AudioIfaceProtocol_t
	{
		IFACE_PROTO_01_00 = 0x00,
		IFACE_PROTO_02_00 = 0x20,
		IFACE_PROTO_03_00 = 0x30
	};

	bool init(uint8_t number, uint8_t altSetting,
			 AudioIfaceSubClass_t subClass,
			 AudioIfaceProtocol_t proto,
			 const UsbStringDescriptor& ifaceStr)
	{
		return XUsbIface::init(number, altSetting, AUDIO_IFACE_CLASS,
							   uint8_t(subClass), uint8_t(proto), ifaceStr);
	}

	inline AudioControlIfaceDescriptor beginAudioControlIface() const
	{
		return CSACDescriptorBase(XUsbIface::beginCSDescriptor());
	}

	inline bool endAudioControlIface(const AudioControlIfaceDescriptor& desc)
	{
		return XUsbIface::endCSDescriptor(desc);
	}

private:
	static const uint8_t AUDIO_IFACE_CLASS = 0x01;
};

#endif /* DEVICE_USBAUDIODEVICE_HPP_ */
