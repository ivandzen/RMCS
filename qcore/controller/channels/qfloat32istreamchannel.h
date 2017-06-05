#ifndef QFLOAT32ISTREAMCHANNEL_H
#define QFLOAT32ISTREAMCHANNEL_H
#include <qcore/controller/qistreamchannelcontroller.h>
#include <core/common/defaultnodes.h>

class QFloat32IStreamChannel :
        public QIStreamChannelController
{
    Q_OBJECT
public:
    QFloat32IStreamChannel(NodeID_t node_id = NodeIDNull,
                           NodeID_t parent_id = NodeIDNull,
                           const QString & name = QString(),
                           QDeviceConnection * connection = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) final override;

    virtual NodeControllerForm * createForm(QWidget * parent) final override;

    virtual NodeType_t type() const final override { return NODE_TYPE_FLOAT32_IPARAM; }

    virtual Data * data() const final override;

protected:
    virtual void toggledEvent(bool enabled) final override;
};

#endif // QFLOAT32ISTREAMCHANNEL_H
