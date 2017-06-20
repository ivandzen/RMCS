#ifndef SUPERVISOR_H
#define SUPERVISOR_H
#include <QThread>
#include <QHash>

class QDeviceControllerFactory;
class QNodeControllerFactory;

class QRmcs :
        public QThread
{
    Q_OBJECT
public:
    virtual ~QRmcs();

    static void init();

    static void finalize();

    static QRmcs * instance() { return _instance; }

    QDeviceControllerFactory * loadFactory(const QString & filename);

    bool addFactory(QDeviceControllerFactory * factory);

    QStringList factoriesNames() const;

    QDeviceControllerFactory * getFactory(const QString & factory_name) const;

    inline QNodeControllerFactory * nodeFactory() const { return _nodeFactory; }

protected:
    virtual void run() override;

private:
    explicit QRmcs(QObject * parent = nullptr);

    QHash<QString, QDeviceControllerFactory*> _devFactories;
    QNodeControllerFactory * _nodeFactory;

    static QRmcs * _instance;
};

#endif // SUPERVISOR_H
