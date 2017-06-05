#include "quint32istreamchannel.h"

QUInt32IStreamChannel::QUInt32IStreamChannel(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString &name,
                                             QDeviceConnection *connection) :
    QIStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt32IStreamChannel::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * device)
{
    return new QUInt32IStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt32IStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

Data *QUInt32IStreamChannel::data() const
{
    return nullptr;
}

void QUInt32IStreamChannel::toggledEvent(bool enabled)
{

}
