#include "qfloat64istreamchannel.h"

QFloat64IStreamChannel::QFloat64IStreamChannel(NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const QString &name,
                                               QDeviceConnection *connection) :
    QIStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QFloat64IStreamChannel::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceConnection * device)
{
    return new QFloat64IStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QFloat64IStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

Data *QFloat64IStreamChannel::data() const
{
    return nullptr;
}

void QFloat64IStreamChannel::toggledEvent(bool enabled)
{

}
