#ifndef SUPERVISOR_H
#define SUPERVISOR_H
#include <QThread>
#include <QHash>

class QDeviceControllerFactory;

class QRmcs :
        public QThread
{
    Q_OBJECT
public:
    explicit QRmcs(QObject * parent = nullptr);

    virtual ~QRmcs();

    QDeviceControllerFactory * loadFactory(const QString & filename);

    bool addFactory(QDeviceControllerFactory * factory);

    QStringList factoriesNames() const;

    QDeviceControllerFactory * getFactory(const QString & factory_name) const;

protected:
    virtual void run() override;

private:
    QHash<QString, QDeviceControllerFactory*> _factories;
};

#endif // SUPERVISOR_H
