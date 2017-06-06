#ifndef QNODECONTROLLERFACTORY_H
#define QNODECONTROLLERFACTORY_H

#include <QObject>
#include <QHash>
#include <qcore/controller/qnodecontroller.h>

class QNodeControllerFactory :
        public QObject
{
    Q_OBJECT
public:
    explicit QNodeControllerFactory(QObject *parent = 0);

    virtual ~QNodeControllerFactory();

    bool loadPrototype(const QString & filename);

    bool addPrototype(QNodeController * node);

    QNodeController * createNode(NodeType_t node_type,
                                 NodeID_t node_id,
                                 NodeID_t parent_id,
                                 const QString &name,
                                 QDeviceConnection *device);

private:
    QHash<NodeType_t, QNodeController*> _prototypes;
};

#endif // QNODECONTROLLERFACTORY_H
