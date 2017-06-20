#ifndef QDEFAULTCONTROLLER_H
#define QDEFAULTCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>

class QDefaultController :
        public QNodeController
{
    Q_OBJECT
public:
    QDefaultController(NodeID_t node_id = NodeIDNull,
                       NodeID_t parent_id = NodeIDNull,
                       const QString & name = QString(),
                       QDeviceConnection * dev = nullptr);

    virtual NodeType_t type() const override;

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

protected:
    virtual bool eventSetup(const ControlPacket & settings) override;

    virtual bool eventData(const ControlPacket & packet) override;
};

#endif // QDEFAULTCONTROLLER_H
