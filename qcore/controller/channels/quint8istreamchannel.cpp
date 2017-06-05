#include "quint8istreamchannel.h"

QUInt8IStreamChannel::QUInt8IStreamChannel(NodeID_t node_id,
                                           NodeID_t parent_id,
                                           const QString & name,
                                           QDeviceConnection * connection) :
    QIStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt8IStreamChannel::createInstance(NodeID_t node_id,
                                                      NodeID_t parent_id,
                                                      const QString & name,
                                                      QDeviceConnection * device)
{
    return new QUInt8IStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt8IStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

Data *QUInt8IStreamChannel::data() const
{
    return nullptr;
}

void QUInt8IStreamChannel::toggledEvent(bool enabled)
{

}
