#include "quint16istreamchannel.h"

QUInt16IStreamChannel::QUInt16IStreamChannel(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString &name,
                                             QDeviceConnection *connection) :
    QIStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt16IStreamChannel::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * device)
{
    return new QUInt16IStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt16IStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

Data *QUInt16IStreamChannel::data() const
{
    return nullptr;
}

void QUInt16IStreamChannel::toggledEvent(bool enabled)
{

}
