#ifndef QDEVICECONTROLLERFACTORY_H
#define QDEVICECONTROLLERFACTORY_H
#include <QStringList>
#include <QObject>
#include <QtPlugin>
#include <qcore/controller/qrmcs.h>

class QDeviceController;
class QWidget;

class QDeviceControllerFactory :
        public QObject
{
    Q_OBJECT
public:
    explicit QDeviceControllerFactory(QObject * parent = nullptr) :
        QObject(parent),
        _context(nullptr)
    {}

    void setContext(QRmcs * rmcs)
    {
        if(_context != nullptr)
            return;
        _context = rmcs;
        moveToThread(rmcs);
    }

    inline QRmcs * getContext() const { return _context; }

    virtual ~QDeviceControllerFactory() {}

    virtual void refresh() = 0;

    virtual QString factoryName() const = 0;

    virtual QStringList devicesNames() const = 0;

    virtual QDeviceController * getDevice(const QString & device_name) = 0;

    virtual QWidget * controlWidget() const = 0;

    class QRmcs * _context;
};

Q_DECLARE_INTERFACE(QDeviceControllerFactory, "RMCS.QDeviceControllerFactory");

#endif // QDEVICECONTROLLERFACTORY_H
