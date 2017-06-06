#include "usbdescriptors.h"

void UsbDesc::pushDescriptor(const UsbDesc *descriptor) {
    uint8_t * new_data = new uint8_t[_length + descriptor->_length];
    uint16_t old_length = _length;
    memcpy(new_data, _data, old_length);
    memcpy(new_data + old_length, descriptor->_data, descriptor->_length);
    delete[] _data;
    _data = new_data;
    _length = _length + descriptor->_length;
}

void ConfigDesc::addInterface(const IfaceDesc &iface_desc)
{
    pushDescriptor(&iface_desc);
    setByte(ConfigDesc::bNumInterfaces, getByte(ConfigDesc::bNumInterfaces) + 1);
    setWord(ConfigDesc::wTotalLength, getWord(ConfigDesc::wTotalLength) + iface_desc.length());
}

void IfaceDesc::addEndpoint(const EndpDesc &ep_desc)
{
    pushDescriptor(&ep_desc);
    setByte(bNumEndpoints, getByte(bNumEndpoints) + 1);
}
