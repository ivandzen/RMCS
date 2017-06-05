#include "istreamchannel.h"
#include <core/common/control_protocol.h>
#include <core/device/istreamnode.h>
#include "device.h"

IStreamChannel::IStreamChannel(NodeType_t type,
                               const char * name,
							   IStreamNode * istream,
                               Length_t data_size,
                               Node * parent) :
    Node (type, name, parent),
    _enabled(true),
    _istream(istream),
    _offset(0),
    _length(data_size)
{
    _istream->addChannel(this);
}

IStreamChannel::IStreamChannel(NodeType_t type,
                               const char * name,
                               IStreamNode * istream,
                               Length_t data_size,
                               Device * dev) :
    Node(type, name, dev),
    _enabled(true),
    _istream(istream),
    _offset(0),
    _length(data_size)
{
    _istream->addChannel(this);
}

bool IStreamChannel::settingsRequested(ControlPacket & packet) const
{
    return StreamChannelSettings(packet).
            init(_istream->id(), _offset, _length);
}

bool IStreamChannel::nodeDataRequested(ControlPacket & packet) const
{
    DataPacket<bool> channel_state(packet);
    return channel_state.init(_enabled);
}

bool IStreamChannel::nodeDataReceived(const ControlPacket &packet)
{
    DataPacket<bool> channel_state(packet);

    if(!channel_state.isValid())
    {
        //! @todo printLog("nodeDataReceived : Invalid packet");
        return false;
    }

    _enabled = *channel_state.get();
    return true;
}
