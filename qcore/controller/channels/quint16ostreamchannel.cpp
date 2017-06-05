#include "quint16ostreamchannel.h"

QUInt16OStreamChannel::QUInt16OStreamChannel(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString &name,
                                             QDeviceConnection *connection) :
    QOStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt16OStreamChannel::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * device)
{
    return new QUInt16OStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt16OStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

void QUInt16OStreamChannel::streamDataReceived(Data *data)
{

}

void QUInt16OStreamChannel::toggledEvent(bool enabled)
{

}
