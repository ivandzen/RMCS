#ifndef QFLOAT64OSTREAMCHANNEL_H
#define QFLOAT64OSTREAMCHANNEL_H
#include <qcore/controller/qostreamchannelcontroller.h>
#include <core/common/defaultnodes.h>

class QFloat64OStreamChannel :
        public QOStreamChannelController
{
    Q_OBJECT
public:
    QFloat64OStreamChannel(NodeID_t node_id = NodeIDNull,
                           NodeID_t parent_id = NodeIDNull,
                           const QString & name = QString(),
                           QDeviceConnection * connection = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) final override;

    virtual NodeControllerForm * createForm(QWidget * parent) final override;

    virtual NodeType_t type() const final override { return NODE_TYPE_FLOAT64_OPARAM; }

    virtual void streamDataReceived(Data * data) final override;

protected:
    virtual void toggledEvent(bool enabled) final override;
};

#endif // QFLOAT64OSTREAMCHANNEL_H
