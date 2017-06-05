#include "boolproperty.h"
#include <core/common/defaultnodes.h>

BoolProperty::BoolProperty(NodeID_t node_id,
                           NodeID_t parent_id,
                           const QString & name,
                           QDeviceConnection * dev) :
    QNodeController (node_id, parent_id, name, dev)
{

}

QNodeController *BoolProperty::createInstance(NodeID_t node_id,
                                              NodeID_t parent_id,
                                              const QString & name,
                                              QDeviceConnection * device)
{
    return new BoolProperty(node_id, parent_id, name, device);
}

NodeControllerForm *BoolProperty::createForm(QWidget *parent)
{
    //! @todo
}

NodeType_t BoolProperty::type() const
{
    return NODE_TYPE_BOOL_PROP;
}

bool BoolProperty::eventSetup(const ControlPacket &)
{
    return true;
}

bool BoolProperty::eventData(const ControlPacket &packet)
{
    //! @todo
}
