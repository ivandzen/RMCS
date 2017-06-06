#include "qusbdevicefactory.h"
#include <QMutexLocker>
#include <usb/host/qusbconnection.h>
#include <qcore/controller/qdevicecontroller.h>
#include <QDebug>
#include "qusbistreamcontroller.h"
#include "qusbostreamcontroller.h"

QUsbCoreThread::QUsbCoreThread(QUsbDeviceFactory *factory) :
    QThread(factory),
    _factory(factory),
    _running(false)
{

}

void QUsbCoreThread::stop()
{
    QMutexLocker locker(&_mutex);
    _running = false;
}

void QUsbCoreThread::run()
{
    qDebug() << "USB core thread started";
    _running = true;
    while(true)
    {
        int res = libusb_handle_events(_factory->_context);

        {
            QMutexLocker locker(&_mutex);
            if(res != 0 || !_running)
            {
                 _running = false;
                break;
            }
        }
    }
}

class QUsbDevice
{
public:
    QUsbDevice(libusb_device * inst) :
        _instance(inst)
    {}

    inline libusb_device * instance() const { return _instance; }

    inline bool operator==(const QUsbDevice & other) const {
        return _instance == other._instance;
    }

    inline bool operator==(libusb_device * other) const {
        return _instance == other;
    }

    inline uint8_t address() const {
        return libusb_get_device_address(_instance);
    }

    inline uint8_t bus() const {
        return libusb_get_bus_number(_instance);
    }

    inline QString name() const {
        return QString("%1:%2").arg(bus()).arg(address());
    }

private:
    libusb_device * _instance;
};

inline uint qHash(const QUsbDevice & device) { return qHash(device.instance()); }

QUsbDeviceFactory::QUsbDeviceFactory(QObject * parent) :
    QDeviceControllerFactory(parent),
    _coreThread(new QUsbCoreThread(this))
{
    libusb_init(&_context);
    refresh();
    _coreThread->start();
}

QUsbDeviceFactory::~QUsbDeviceFactory()
{
    _coreThread->stop();
    _coreThread->wait();
    libusb_exit(_context);
}

QList<QUsbDevice> QUsbDeviceFactory::devices() const
{
    return _connections.keys();
}

QStringList QUsbDeviceFactory::devicesNames() const
{
    QStringList result;
    result.reserve(_connections.size());
    QList<QUsbDevice> devs = devices();
    for(const auto & dev : devs)
        result.push_back(dev.name());
    return result;
}

QUsbConnection * QUsbDeviceFactory::open(QUsbDevice device)
{
    if(!_connections.contains(device))
        return nullptr;

    if(_connections[device])
        return _connections[device];

    libusb_device_handle * new_handle;
    int result;

    result = libusb_open(device.instance(), &new_handle);
    if(result != 0)
    {
        return nullptr;
    }

    result = libusb_set_configuration(new_handle,
                                      ISOUSB_BASE_CONFIG);
    if(result != 0)
    {
        libusb_close(new_handle);
        return nullptr;
    }

    result = libusb_claim_interface(new_handle,
                                    ISOUSB_BASE_INTERFACE);
    if(result != 0)
    {
        libusb_close(new_handle);
        return nullptr;
    }

    result = libusb_set_interface_alt_setting(new_handle,
                                              ISOUSB_BASE_INTERFACE, 0);
    if(result != 0)
    {
        libusb_close(new_handle);
        return nullptr;
    }

    qDebug() << QString("USB device '%1' connected").arg(device.name());
    QUsbConnection * new_connection = new QUsbConnection(new_handle, this);
    _connections[device] = new_connection;
    return new_connection;
}

void QUsbDeviceFactory::refresh()
{
    libusb_device ** device_list;
    ssize_t num_devices = libusb_get_device_list(_context, &device_list);

    QList<QUsbDevice> exists = _connections.keys();
    for(QUsbDevice & dev : exists)
    {
        int i;
        for(i = 0; i < num_devices; ++i)
            if(dev == device_list[i])
                break;
        if(i == num_devices)
            closeConnection(dev);
    }

    for(int i = 0; i < num_devices; ++i)
    {
        libusb_device_descriptor descriptor;
        libusb_get_device_descriptor(device_list[i], &descriptor);
        if((descriptor.idVendor != ISOUSB_VID) ||
           (descriptor.idProduct != ISOUSB_PID))
            continue;
        _connections.insert(device_list[i], nullptr);
    }

    libusb_free_device_list(device_list, 0);

    emit updated();
}

QString QUsbDeviceFactory::factoryName() const
{
    return "USB";
}

QDeviceController *QUsbDeviceFactory::open(QNodeControllerFactory * factory,
                                           const QString & device_name)
{
    QList<QUsbDevice> devs = devices();
    for(auto & dev : devs)
        if(dev.name() == device_name)
        {
            QUsbConnection * new_connection = open(dev);
            if(new_connection == nullptr)
                return nullptr;
            QDeviceController * new_controller = new QDeviceController(new_connection, factory, this);
            return new_controller;
        }
    return nullptr;
}

void QUsbDeviceFactory::closeConnection(const QUsbDevice & dev)
{
    if(!_connections.contains(dev))
        return;

    if(_connections[dev])
    {
        //! @todo
    }
    _connections.remove(dev);
}

