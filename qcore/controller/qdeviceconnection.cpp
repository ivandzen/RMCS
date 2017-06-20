#include "qdeviceconnection.h"
#include <qcore/controller/qdevicecontroller.h>

QDeviceConnection::QDeviceConnection(QObject * parent) :
    QObject(parent)
{

}

bool QDeviceConnection::setController(QDeviceController * controller)
{
    if(!DeviceConnection::setController(controller))
        return false;
    return true;
}

QDeviceController *QDeviceConnection::controller() const {
    return static_cast<QDeviceController*>(DeviceConnection::controller());
}
