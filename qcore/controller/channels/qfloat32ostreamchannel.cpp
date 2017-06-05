#include "qfloat32ostreamchannel.h"

QFloat32OStreamChannel::QFloat32OStreamChannel(NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const QString &name,
                                               QDeviceConnection *connection) :
    QOStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QFloat32OStreamChannel::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceConnection * device)
{
    return new QFloat32OStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QFloat32OStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

void QFloat32OStreamChannel::streamDataReceived(Data *data)
{

}

void QFloat32OStreamChannel::toggledEvent(bool enabled)
{

}
