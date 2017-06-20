#ifndef QDEVICECONTROLLERWRAPPER_H
#define QDEVICECONTROLLERWRAPPER_H
#include <qcore/controller/qdevicecontroller.h>

class QDeviceControllerWrapper :
        public QObject
{
    Q_OBJECT
public slots:
    bool init(QDeviceController * controller) {
        return controller->init();
    }

    bool fullUpdate(QDeviceController * controller) {
        return controller->fullUpdate();
    }

    QNodeController * getNode(QDeviceController * controller,
                              NodeID_t node_id) const {
        return controller->getNode(node_id);
    }

    QNodeController * getNode(QDeviceController * controller,
                              const QString & name) const {
        return controller->getNode(name);
    }

    size_t numNodes(QDeviceController * controller) const
    {
        return controller->numNodes();
    }

    QStringList nodeNames(QDeviceController * controller) const
    {
        return controller->nodeNames();
    }

    bool isNodeExists(QDeviceController * controller,
                      NodeID_t node_id) const
    {
        return controller->isNodeExists(node_id);
    }

    void update(QDeviceController * controller) {
        controller->update();
    }
};

#endif // QDEVICECONTROLLERWRAPPER_H
