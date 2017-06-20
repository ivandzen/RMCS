#include "defaultnode.h"
#include <core/common/defaultnodes.h>
#include <core/common/control_protocol.h>

DefaultNode::DefaultNode(const char * name,
                         Node * parent) :
    Node(NODE_TYPE_DEFAULT, name, parent)
{

}

DefaultNode::DefaultNode(const char * name,
                         Device * device) :
    Node(NODE_TYPE_DEFAULT, name, device)
{

}

bool DefaultNode::settingsRequested(ControlPacket &packet) const
{
    return packet.init(0);
}

bool DefaultNode::nodeDataRequested(ControlPacket & packet) const
{
    return packet.init(0);
}

bool DefaultNode::nodeDataReceived(const ControlPacket &)
{
    return true;
}
