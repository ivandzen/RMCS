#include "istreamnode.h"
#include "nodes.h"
#include "istreamchannel.h"

IStreamNode::IStreamNode(const char * name,
                         NodeType_t type,
                         Length_t num_packets,
                         Device * dev) :
    Node(type, name, dev),
    _enabled(false),
	_numPackets(num_packets),
    _packetSize(0)
{
}

bool IStreamNode::addChannel(IStreamChannel * channel)
{
    if(nodeStatus() != NODE_STAT_UNDEFINED)
        return false;
    channel->_offset = _packetSize;
    _packetSize += channel->dataLength();
    _channels.push_back(channel);
    return true;
}

bool IStreamNode::nodeDataRequested(ControlPacket & packet) const
{
    DataPacket<bool> data(packet);
    return data.init(_enabled);
}

bool IStreamNode::nodeDataReceived(const ControlPacket & packet)
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

void IStreamNode::streamDataReceived(const Data * data,
                                     Length_t length)
{
    if(!_enabled)
        return;

    Length_t offset = 0;
    for(auto it = _channels.begin();
        (offset < length) && (it != _channels.end()); ++it)
    {
        Length_t data_length = (*it)->dataLength();
        (*it)->streamDataReceived(ArrayRef<Data>(data + offset, data_length));
        offset += data_length;
    }
}

bool IStreamNode::init()
{
    if(!Node::init())
        return false;
    _buffer.resize(_numPackets * _packetSize);
    return true;
}
