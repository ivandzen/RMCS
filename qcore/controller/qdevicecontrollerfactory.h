#ifndef QDEVICECONTROLLERFACTORY_H
#define QDEVICECONTROLLERFACTORY_H
#include <QStringList>
#include <QObject>
#include <QtPlugin>


class QNodeControllerFactory;
class QDeviceController;
class QWidget;

class QDeviceControllerFactory :
        public QObject
{
    Q_OBJECT
public:
    explicit QDeviceControllerFactory(QObject * parent = nullptr) :
        QObject(parent)
    {}

    virtual ~QDeviceControllerFactory() {}

    virtual void refresh() = 0;

    virtual QString factoryName() const = 0;

    virtual QStringList devicesNames() const = 0;

    virtual QDeviceController * open(QNodeControllerFactory * factory,
                                     const QString & device_name) = 0;

    virtual QWidget * controlWidget() const = 0;
};

Q_DECLARE_INTERFACE(QDeviceControllerFactory, "RMCS.QDeviceControllerFactory");

#endif // QDEVICECONTROLLERFACTORY_H
