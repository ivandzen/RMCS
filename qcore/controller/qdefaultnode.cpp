#include "qdefaultnode.h"

QDefaultNode::QDefaultNode(NodeID_t node_id,
                           NodeID_t parent_id,
                           const QString & name,
                           QDeviceConnection * dev) :
    QNodeController (node_id,
                     parent_id,
                     name,
                     dev)
{

}

bool QDefaultNode::eventSetup(const ControlPacket &settings)
{
    return true;
}

bool QDefaultNode::eventData(const ControlPacket &packet)
{
    return true;
}
