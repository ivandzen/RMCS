#ifndef QDEFAULTNODE_H
#define QDEFAULTNODE_H
#include <qcore/controller/qnodecontroller.h>
#include <core/common/defaultnodes.h>

class QDefaultNode :
        public QNodeController
{
public:
    QDefaultNode(NodeID_t node_id = NodeIDNull,
                 NodeID_t parent_id = NodeIDNull,
                 const QString & name = QString(),
                 QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) final override
    {
        return new QDefaultNode(node_id, parent_id, name, device);
    }

    virtual NodeControllerForm * createForm(QWidget * parent) final override
    {
        Q_UNUSED(parent);
        return nullptr;
    }

    virtual NodeType_t type() const final override { return NODE_TYPE_DEFAULT; }

protected:
    virtual bool eventSetup(const ControlPacket & settings) final override;

    virtual bool eventData(const ControlPacket & packet) final override;
};

#endif // QDEFAULTNODE_H
