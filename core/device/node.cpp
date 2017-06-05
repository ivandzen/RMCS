#include "node.h"
#include "device.h"
#include <core/common/control_protocol.h>

Node::Node(NodeType_t type,
           const char *name,
           Node *parent) :
    _id(NodeIDNull),
    _type(type),
    _name(name),
    _parent(parent),
    _device(nullptr),
    _status(NODE_STAT_OK)
{
    assert(_parent);
    _device = _parent->_device;//();
    assert(_device);
    _device->addNode(this);
}

Node::Node(NodeType_t type,
           const char *name,
           Device *device) :
    _id(NodeIDNull),
    _type(type),
    _name(name),
    _parent(nullptr),
    _device(device),
    _status(NODE_STAT_UNDEFINED)
{
    assert(device);
    _device->addNode(this);
}

Node::~Node()
{
    //! @todo nothing?
}

bool Node::init()
{
    if(_status != NODE_STAT_UNDEFINED)
        return false;
    _status = NODE_STAT_OK;
    return true;
}

bool Node::controlPacketRequested(CtlReq_t req,
                                    ControlPacket & packet)
{
    switch(req)
    {
    case CTLREQ_NODE_DESCRIPTOR :
        return descriptorRequested(packet);

    case CTLREQ_NODE_SETTINGS :
        return settingsRequested(packet);

    case CTLREQ_NODE_STATUS :
        return nodeStatusRequested(packet);

    case CTLREQ_NODE_DATA :
        return nodeDataRequested(packet);

    case CTLREQ_NODE_RESET :
    {
        reset();
        return true;
    }
    }
    setStatus(NODE_STAT_ERR);
    printLog("Unknown control packet requested");
    return false;
}

bool Node::controlPacketReceived(CtlReq_t req,
                                 const ControlPacket &packet)
{
    switch(req)
    {
    case CTLREQ_NODE_DATA :
        return nodeDataReceived(packet);
    }
    printLog("Unknown control packet received");
    setStatus(NODE_STAT_ERR);
    return false;
}

void Node::reset()
{
    _status = NODE_STAT_OK;
}

bool Node::descriptorRequested(ControlPacket & packet) const
{
    NodeDescriptor descriptor(packet);
    return descriptor.init(_id,
                           parent() ? parent()->id() : NodeIDNull,
                           _type, _name);
}

bool Node::nodeStatusRequested(ControlPacket &packet) const
{
    DataPacket<NodeStatus_t> st(packet);
    return st.init(_status);
}
