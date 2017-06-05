#include "qfloat32istreamchannel.h"

QFloat32IStreamChannel::QFloat32IStreamChannel(NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const QString &name,
                                               QDeviceConnection *connection) :
    QIStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QFloat32IStreamChannel::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceConnection * device)
{
    return new QFloat32IStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QFloat32IStreamChannel::createForm(QWidget * parent)
{
    return nullptr;
}

Data *QFloat32IStreamChannel::data() const
{
    return nullptr;
}

void QFloat32IStreamChannel::toggledEvent(bool enabled)
{

}
