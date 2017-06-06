#include "usbdeviceep.h"
#include <usb/device/usbdevice.h>

UsbDeviceEP::UsbDeviceEP(UsbDeviceEP::Type type,
                         uint8_t addr,
                         uint8_t attributes,
                         uint8_t iso_interval,
                         UsbDevice * device) :
    _type(type),
    _addr(addr),
    _attributes(attributes),
    _isoInterval(iso_interval),
    _devHandle(device->handle())
{}
