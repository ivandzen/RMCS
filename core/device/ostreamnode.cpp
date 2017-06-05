#include "ostreamnode.h"
#include "nodes.h"

OStreamNode::OStreamNode(const char * name,
                         NodeType_t type,
                         Device * dev) :
    Node(type, name, dev),
    _packetSize(0)
{
}

bool OStreamNode::addChannel(OStreamChannel * channel)
{
    if(nodeStatus() != NODE_STAT_UNDEFINED)
        return false;
    channel->_offset = _packetSize;
    _packetSize += channel->dataLength();
    return true;
}

bool OStreamNode::nodeDataRequested(ControlPacket & packet) const
{
    DataPacket<bool> data(packet);
    return data.init(_enabled);
}

bool OStreamNode::nodeDataReceived(const ControlPacket & packet)
{
    DataPacket<bool> data(packet);
    if(!data.isValid())
        return false;
    if(_enabled != *data.get())
    {
        _enabled = *data.get();
        streamToggled(_enabled);
    }
    return true;
}

bool OStreamNode::init()
{
    if(!Node::init())
        return false;
    _buffer.resize(_packetSize);
    return true;
}
