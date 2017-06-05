#include "ostreamchannel.h"
#include "device.h"
#include <core/common/control_protocol.h>
#include <core/device/ostreamnode.h>

OStreamChannel::OStreamChannel(NodeType_t type,
                               const char * name,
							   OStreamNode * ostream,
                               Length_t data_size,
                               Node * parent) :
    Node(type, name, parent),
    _ostream(ostream),
    _enabled(true),
    _offset(0),
    _length(data_size)
{
    _ostream->addChannel(this);
}

OStreamChannel::OStreamChannel(NodeType_t type,
                               const char * name,
							   OStreamNode * ostream,
                               Length_t data_size,
                               Device * dev) :
    Node(type, name, dev),
    _ostream(ostream),
    _enabled(true),
    _offset(0),
    _length(data_size)
{
    _ostream->addChannel(this);
}

bool OStreamChannel::settingsRequested(ControlPacket &packet) const
{
    return StreamChannelSettings(packet).
            init(_ostream->id(), _offset, _length);
}

bool OStreamChannel::nodeDataRequested(ControlPacket &packet) const
{
    DataPacket<bool> channel_state(packet);
    return channel_state.init(_enabled);
}

bool OStreamChannel::nodeDataReceived(const ControlPacket &packet)
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
