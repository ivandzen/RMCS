#ifndef QNODECONTROLLERFACTORYWRAPPER_H
#define QNODECONTROLLERFACTORYWRAPPER_H
#include <qcore/controller/qnodecontrollerfactory.h>

class QNodeControllerFactoryWrapper :
        public QObject
{
    Q_OBJECT
public Q_SLOTS :
    bool loadPrototype(QNodeControllerFactory * factory,
                       const QString & filename) {
        return factory->loadPrototype(filename);
    }

    bool addPrototype(QNodeControllerFactory * factory,
                      QNodeController * node) {
        return factory->addPrototype(node);
    }

    QNodeController * createNode(QNodeControllerFactory * factory,
                                 NodeType_t node_type,
                                 NodeID_t node_id,
                                 NodeID_t parent_id,
                                 const QString &name,
                                 QDeviceConnection *device) const {
        return factory->createNode(node_type, node_id, parent_id,
                                   name, device);
    }
};

#endif // QNODECONTROLLERFACTORYWRAPPER_H
