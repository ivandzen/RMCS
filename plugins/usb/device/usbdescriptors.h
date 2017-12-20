#ifndef DESCRIPTORS_H
#define DESCRIPTORS_H
#include <stdint.h>
#include <string.h>
#include <core/common/control_protocol.h>

class UsbDescriptor :
        public ArrayRef<Data>
{
public:
    enum Type {
        DescDevice          = 0x01,
        DescConfiguration   = 0x02,
        DescString          = 0x03,
        DescInterface       = 0x04,
        DescEndpoint        = 0x05,
        DescDeviceQualifier = 0x06,
        DescOtherSpeedConfiguration = 0x07,
        DescBOS             = 0x0F
    };

    UsbDescriptor(Data * data, Length_t size) :
        ArrayRef<Data>(data, size)
    {
        header()->bLength = 0;
    }

    UsbDescriptor(const UsbDescriptor & other) :
        ArrayRef<Data>(other)
    {}

    inline bool isValid() const
    {
        return bLength() < size();
    }

    inline bool init(uint8_t length,
                     uint8_t desc_type)
    {
        if(length > size())
            return false;
        header()->bLength = length;
        header()->bDescriptorType = desc_type;
        return true;
    }

    inline uint8_t bLength() const { return header()->bLength; }

    inline uint8_t bDescriptorType() const { return header()->bDescriptorType; }

protected:
    bool pushDescriptor(const UsbDescriptor & desc)
    {
        if(desc.bLength() + bLength() > size())
            return false;
        memcpy(data() + bLength(), desc.data(), desc.size());
        header()->bLength += desc.size();
        return true;
    }

private:
    typedef struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t fields[];
    }UsbDescriptorHeader;

    inline UsbDescriptorHeader * header() const
    {
        return reinterpret_cast<UsbDescriptorHeader*>(ArrayRef<Data>::data());
    }

    inline uint8_t * fields() const
    {
        return header()->fields;
    }

    inline Data * data() const
    {
        return ArrayRef<Data>::data() + sizeof(UsbDescriptorHeader);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class UsbEndpointDescriptor :
        public UsbDescriptor
{
private:
    typedef struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bEndpointAddress;
        uint8_t bmAttributes;
        uint16_t wMaxPacketSize;
        uint8_t bInterval;
    } __attribute__((packed))
    EndpointDescriptorFields ;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class UsbInterfaceDescriptor :
        public UsbDescriptor
{
private:
    typedef struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bInterfaceNumber;
        uint8_t bAlternateSetting;
        uint8_t bNumEndpoints;
        uint8_t bInterfaceClass;
        uint8_t bInterfaceSubClass;
        uint8_t bInterfaceProtocol;
        uint8_t iInterface;
    } __attribute__((packed))
    InterfaceDescriptorFields;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class UsbConfigDescriptor :
        public UsbDescriptor
{
private:
    typedef struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t wTotalLength;
        uint8_t bNumInterfaces;
        uint8_t bConfigurationValue;
        uint8_t iConfiguration;
        uint8_t bmAttributes;
        uint8_t bMaxPower;
    } __attribute__((packed))
    ConfigDescriptorFields;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class UsbDeviceDescriptor :
        public UsbDescriptor
{
private:
    typedef struct
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint16_t bcdUSB;
        uint8_t bDeviceClass;
        uint8_t bDeviceSubClass;
        uint8_t bDeviceProtocol;
        uint8_t bMaxPacketSize;
        uint16_t idVendor;
        uint16_t idProduct;
        uint16_t bcdDevice;
        uint8_t iManufacturer;
        uint8_t iProduct;
        uint8_t iSerialNumber;
        uint8_t bNumConfigurations;
    } __attribute__((packed))
    UsbDeviceDescriptorFields;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class UsbDesc {
public:
    static const uint8_t bLength = 0;
    static const uint8_t bDescriptorType = 1;

    enum Type {
        DescDevice = 1,
        DescConfiguration = 2,
        DescString = 3,
        DescInterface = 4,
        DescEndpoint = 5,
        DescDeviceQualifier = 6,
        DescOtherSpeedConfiguration = 7,
        DescBOS = 0x0F
    };

    UsbDesc(Type type, uint16_t length) :
        _data(new uint8_t[length]),
        _length(length),
        _allocated(true) {
        _data[bLength] = length & 0xFF;
        _data[bDescriptorType] = type & 0xFF;
    }

    UsbDesc(uint8_t * data, uint16_t length) :
        _data(data),
        _length(length),
        _allocated(false) { }

    virtual ~UsbDesc() {
        if(_allocated)
            delete[] _data;
    }

    inline uint16_t length() const { return _length; }

    void pushDescriptor(const UsbDesc * descriptor);

    inline void setByte(uint8_t index, uint8_t value) { _data[index] = value; }

    inline uint8_t getByte(uint8_t index) const { return _data[index]; }

    inline void setWord(uint8_t index, uint16_t value) {
        _data[index] = value & 0xFF;
        _data[index + 1] = (value >> 8) & 0xFF;
    }

    inline uint16_t getWord(uint8_t index) const {
        uint16_t result = 0x0000;
        result |= _data[index];
        result |= (_data[index + 1]) << 8;
        return result;
    }

    inline uint8_t * operator*() const { return _data; }

private:
    uint8_t * _data;
    uint16_t _length;
    bool _allocated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class DeviceDesc :
        public UsbDesc {
public:
    static const uint8_t bcdUSB = 2;
    static const uint8_t bDeviceClass = 4;
    static const uint8_t bDeviceSubClass = 5;
    static const uint8_t bDeviceProtocol = 6;
    static const uint8_t bMaxPacketSize = 7;
    static const uint8_t idVendor = 8;
    static const uint8_t idProduct = 10;
    static const uint8_t bcdDevice = 12;
    static const uint8_t iManufacturer = 14;
    static const uint8_t iProduct = 15;
    static const uint8_t iSerialNumber = 16;
    static const uint8_t bNumConfigurations = 17;

    enum BCDUsb {
        USB_1_0 = 0x0100,
        USB_1_1 = 0x0101,
        USB_2_0 = 0x0200,
        USB_2_1 = 0x0201
    };

    enum EP0PacketSize {
        EP0PackSize8 = 8,
        EP0PackSize16 = 16,
        EP0PackSize32 = 32,
        EP0PackSize64 = 64,
    };

    DeviceDesc() :
        UsbDesc(DescDevice, 18) {}

    DeviceDesc(uint8_t * data, uint16_t length) :
        UsbDesc(data, length) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class IfaceDesc;

class ConfigDesc :
        public UsbDesc {
public:
    static const uint8_t wTotalLength = 2;
    static const uint8_t bNumInterfaces = 4;
    static const uint8_t bConfigurationValue = 5;
    static const uint8_t iConfiguration = 6;
    static const uint8_t bmAttributes = 7;
    static const uint8_t bMaxPower = 8;

    ConfigDesc() :
        UsbDesc(DescConfiguration, 9) {
        setWord(wTotalLength, 9);
        setByte(bNumInterfaces, 0x00);
    }

    ConfigDesc(uint8_t * data, uint16_t length) :
        UsbDesc(data, length) {}

    void addInterface(const IfaceDesc & iface_desc);

    uint16_t totalLength() const { return getWord(wTotalLength); }

    uint8_t numInterfaces() const { return getByte(bNumInterfaces); }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class EndpDesc;

class IfaceDesc :
        public UsbDesc {
public:
    static const uint8_t bInterfaceNumber = 2;
    static const uint8_t bAlternateSetting = 3;
    static const uint8_t bNumEndpoints = 4;
    static const uint8_t bInterfaceClass = 5;
    static const uint8_t bInterfaceSubClass = 6;
    static const uint8_t bInterfaceProtocol = 7;
    static const uint8_t iInterface = 8;

    IfaceDesc() :
        UsbDesc(DescInterface, 9) {
        setByte(bNumEndpoints, 0x00);
    }

    IfaceDesc(uint8_t * data, uint16_t length) :
        UsbDesc(data, length) {}

    IfaceDesc(const UsbDesc & desc) :
        UsbDesc(*desc, desc.length()) {}

    void addEndpoint(const EndpDesc & ep_desc);
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class EndpDesc :
        public UsbDesc {
public:
    static const uint8_t bEndpointAddress = 2;
    static const uint8_t bmAttributes = 3;
    static const uint8_t wMaxPacketSize = 4;
    static const uint8_t bInterval = 6;

    EndpDesc() :
        UsbDesc(DescEndpoint, 7) {}

    EndpDesc(uint8_t * data, uint16_t length) :
        UsbDesc(data, length) {}

    EndpDesc(const UsbDesc & desc) :
        UsbDesc(*desc, desc.length()) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class LangIDDesc :
        public UsbDesc {
public:
    static const uint8_t wLANGID = 2;

    LangIDDesc(uint16_t langid) :
        UsbDesc(DescString, 4) {
        setWord(wLANGID, langid);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class StringDesc :
        public UsbDesc {
public:
    static const uint8_t bString = 2;
    explicit StringDesc(const char * const str) :
        UsbDesc(DescString, strlen(str) * 2 + bString) {
        for(uint16_t i = 0; i < strlen(str); i++) {
            setByte(bString + i * 2, str[i]);
            setByte(bString + i * 2 + 1, 0x00);
        }
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class BOSDesc :
        public UsbDesc {
public:
    static const uint8_t bDevCapabilityType = 2;
    static const uint8_t bDevCapabilityData = 3;
    explicit BOSDesc(uint8_t data_len) :
        UsbDesc(DescBOS, data_len + 3) { }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // DESCRIPTORS_H
