#include "quint32ostreamchannel.h"

QUInt32OStreamChannel::QUInt32OStreamChannel(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString &name,
                                             QDeviceConnection *connection) :
    QOStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt32OStreamChannel::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * device)
{
    return new QUInt32OStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt32OStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

void QUInt32OStreamChannel::streamDataReceived(Data *data)
{

}

void QUInt32OStreamChannel::toggledEvent(bool enabled)
{

}
