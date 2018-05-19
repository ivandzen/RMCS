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
