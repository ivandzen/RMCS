#ifndef QRMCSWRAPPER_H
#define QRMCSWRAPPER_H
#include <qcore/controller/qrmcs.h>

class QRmcsWrapper :
        public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void static_QRmcs_init() {
        QRmcs::init();
    }

    void static_QRmcs_finalize() {
        QRmcs::finalize();
    }

    QRmcs * static_QRmcs_instance() {
        return QRmcs::instance();
    }

    bool loadFactory(QRmcs * rmcs,
                     const QString & filename) {
        return rmcs->loadFactory(filename);
    }

    bool addFactory(QRmcs * rmcs,
                    QDeviceControllerFactory * factory) {
        return rmcs->addFactory(factory);
    }

    QStringList factoriesNames(QRmcs * rmcs) const {
        return rmcs->factoriesNames();
    }

    QDeviceControllerFactory * getFactory(QRmcs * rmcs,
                                          const QString & factory_name) const {
        return rmcs->getFactory(factory_name);
    }

    QNodeControllerFactory * nodeFactory(QRmcs * rmcs) const {
        return rmcs->nodeFactory();
    }
};

#endif // QRMCSWRAPPER_H
