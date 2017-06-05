#include "qfloat64ostreamchannel.h"

QFloat64OStreamChannel::QFloat64OStreamChannel(NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const QString &name,
                                               QDeviceConnection *connection) :
    QOStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QFloat64OStreamChannel::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceConnection * device)
{
    return new QFloat64OStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QFloat64OStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

void QFloat64OStreamChannel::streamDataReceived(Data *data)
{

}

void QFloat64OStreamChannel::toggledEvent(bool enabled)
{

}
