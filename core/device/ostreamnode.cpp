#include "ostreamnode.h"
#include "nodes.h"

OStreamNode::OStreamNode(const char * name,
                         NodeType_t type,
                         Device * dev,
						 uint16_t maxPacketSize) :
    Node(type, name, dev),
	_enabled(false),
    _packetSize(0)
{
	_buffer.resize(maxPacketSize);
}

bool OStreamNode::addChannel(OStreamChannel * channel)
{
    if(nodeStatus() != NODE_STAT_UNDEFINED)
        return false;
    assert(_packetSize + channel->dataLength() < _buffer.size());
    channel->_offset = _packetSize;
    _packetSize += channel->dataLength();
    return true;
}

bool OStreamNode::nodeDataRequested(ControlPacket & packet) const
{
    (void)packet;
    return true;
}

bool OStreamNode::nodeDataReceived(const ControlPacket & packet)
{
    (void)packet;
    return true;
}

bool OStreamNode::toggleStream(bool enabled)
{
    if(_enabled != enabled)
    {
        _enabled = enabled;
        streamToggled(_enabled);
    }
    return true;
}
