#include "qdefaultcontroller.h"
#include <core/common/defaultnodes.h>

QDefaultController::QDefaultController(NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * dev) :
    QNodeController (node_id, parent_id, name, dev)
{
}

NodeType_t QDefaultController::type() const
{
    return NODE_TYPE_DEFAULT;
}

QNodeController *QDefaultController::createInstance(NodeID_t node_id,
                                                    NodeID_t parent_id,
                                                    const QString & name,
                                                    QDeviceConnection * device)
{
    return new QDefaultController(node_id, parent_id, name, device);
}

bool QDefaultController::eventSetup(const ControlPacket &settings)
{
    (void)settings;
    return true;
}

bool QDefaultController::eventData(const ControlPacket &packet)
{
    (void)packet;
    return true;
}
