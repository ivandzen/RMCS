#include "quint8ostreamchannel.h"

QUInt8OStreamChannel::QUInt8OStreamChannel(NodeID_t node_id,
                                           NodeID_t parent_id,
                                           const QString &name,
                                           QDeviceConnection *connection) :
    QOStreamChannelController (node_id, parent_id, name, connection)
{

}

QNodeController *QUInt8OStreamChannel::createInstance(NodeID_t node_id,
                                                      NodeID_t parent_id,
                                                      const QString &name,
                                                      QDeviceConnection *device)
{
    return new QUInt8OStreamChannel(node_id, parent_id, name, device);
}

NodeControllerForm *QUInt8OStreamChannel::createForm(QWidget *parent)
{
    return nullptr;
}

void QUInt8OStreamChannel::streamDataReceived(Data *data)
{

}

void QUInt8OStreamChannel::toggledEvent(bool enabled) {}
