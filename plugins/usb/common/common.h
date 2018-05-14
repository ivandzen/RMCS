#ifndef ISOUSB_COMMON_H
#define ISOUSB_COMMON_H
#include <core/common/types.h>
#include <plugins/usb/common/usbdescriptors.h>
#include <core/common/control_protocol.h>

#define MAX_ISO_PACKET_LENGTH           1023
#define MAX_SETUP_DATA_LENGTH			1024

#define ISOUSB_VID                      0x1ead
#define ISOUSB_PID                      0xf1ea

#define ISOUSB_ISOIN_EP                 0x81
#define ISOUSB_ISOOUT_EP                0x01
#define ISOUSB_INTIN_EP                 0x82
#define ISOUSB_INTOUT_EP                0x02

#define ISOUSB_BASE_CONFIG              1
#define ISOUSB_BASE_INTERFACE           0

#ifdef ENABLE_USBDEV

#define EP_ATTRIBUTES (UsbDevice::EP_ISOC | \
                       UsbDevice::SYNC_MODE_NONE | \
                       UsbDevice::USAGE_MODE_DATA)

#endif

#define USBSTREAM_EP_ATTRIBUTES (UsbEPDescriptor::EP_BULK | \
                       	   	   	 UsbEPDescriptor::SYNC_MODE_NONE | \
								 UsbEPDescriptor::USAGE_MODE_DATA)

#define NODE_TYPE_USBISTREAM 45
#define NODE_TYPE_USBOSTREAM 46

class UsbStreamSettings :
        public ControlPacket
{
public:
    UsbStreamSettings(const ControlPacket & other) :
        ControlPacket(other)
    {}

    inline bool init(uint8_t ep_addr,
                     Length_t packet_size)
    {
        if(!ControlPacket::init(sizeof(UsbStreamSettingsHeader)))
            return false;
        header()->ep_addr = ep_addr;
        header()->packet_size = packet_size;
        return true;
    }

    inline uint8_t epAddr() const { return header()->ep_addr; }

    inline Length_t packetSize() const { return header()->packet_size; }

private:
    typedef struct
    {
        uint8_t 	ep_addr;
        Length_t 	packet_size;
    }UsbStreamSettingsHeader;

    inline UsbStreamSettingsHeader * header() const
    {
        return reinterpret_cast<UsbStreamSettingsHeader*>(payload());
    }
};

#endif //ISOUSB_COMMON_H
