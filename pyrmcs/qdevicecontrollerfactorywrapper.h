#ifndef QDEVICECONTROLLERFACTORYWRAPPER_H
#define QDEVICECONTROLLERFACTORYWRAPPER_H
#include <qcore/controller/qdevicecontrollerfactory.h>

class QDeviceControllerFactoryWrapper :
        public QObject
{
    Q_OBJECT
public Q_SLOTS :
    void refresh(QDeviceControllerFactory * factory) {
        factory->refresh();
    }

    QString factoryName(QDeviceControllerFactory * factory) const {
        return factory->factoryName();
    }


    QStringList devicesNames(QDeviceControllerFactory * factory) const {
        return factory->devicesNames();
    }

    QDeviceController * getDevice(QDeviceControllerFactory * dev_factory,
                                  const QString & device_name) {
        return dev_factory->getDevice(device_name);
    }

    QWidget * controlWidget(QDeviceControllerFactory * factory) const {
        return factory->controlWidget();
    }
};

#endif // QDEVICECONTROLLERFACTORYWRAPPER_H
