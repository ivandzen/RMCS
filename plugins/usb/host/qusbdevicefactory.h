#ifndef QUSBCORE_H
#define QUSBCORE_H

#include <QThread>
#include <QMutex>
#include <libusb.h>
#include <QHash>
#include <qcore/controller/qdevicecontrollerfactory.h>

class QUsbDevice;
class QUsbConnection;
class QUsbDeviceFactory;

class QUsbCoreThread :
        public QThread
{
    Q_OBJECT
public:
    explicit QUsbCoreThread(QUsbDeviceFactory * factory);

    void stop();

protected:
    virtual void run() override;

private:
    QUsbDeviceFactory * _factory;
    bool    _running;
    QMutex  _mutex;
};

class QUsbDeviceFactory :
        public QDeviceControllerFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "RMCS.QUsbDeviceFactory")
    Q_INTERFACES(QDeviceControllerFactory)

    friend class QUsbCoreThread;
public:
    explicit QUsbDeviceFactory(QObject * parent = 0);

    virtual ~QUsbDeviceFactory();

    QList<QUsbDevice> devices() const;

    QUsbConnection * open(QUsbDevice device);

    virtual void refresh() override;

    virtual QString factoryName() const override;

    virtual QStringList devicesNames() const override;

    virtual QDeviceController * open(QNodeControllerFactory * factory,
                                     const QString & device_name) override;

    virtual QWidget * controlWidget() const override { return nullptr; }

private:
    typedef struct
    {
        QUsbConnection * connection;
        libusb_device * device;
    }ConnectionContainer;

    void closeConnection(const QUsbDevice & dev);

    QNodeControllerFactory *            _factory;
    QUsbCoreThread *                    _coreThread;
    libusb_context *                    _context;
    QHash<QUsbDevice, QUsbConnection*>  _connections;

signals:
    void updated();
};

#endif // QUSBCORE_H
