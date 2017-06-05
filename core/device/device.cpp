#include "device.h"
#include "node.h"
#include "istreamchannel.h"
#include "ostreamchannel.h"
#include <core/common/control_protocol.h>
#include "istreamnode.h"
#include "ostreamnode.h"

Device::Device(Length_t max_ctl_req_size) :
    _maxCtlReqSize(max_ctl_req_size)
{
    _inputStreams.reserve(15);
    _outputStreams.reserve(15);
}

void Device::addNode(Node * node)
{
    //! @todo удостовериться, что этого достаточно
    _nodes.push_back(node);
    node->_id = NodeID_t(_nodes.size() - 1);
}

bool Device::controlPacketRequested(NodeID_t target,
                                    CtlReq_t req,
                                    ControlPacket & packet)
{
    switch(req)
    {
    case CTLREQ_DEV_DESCRIPTOR :
        return deviceDescriptorRequested(packet);

    default : break;
    }

    if(isNodeExists(target))
        return getNode(target)->controlPacketRequested(req, packet);

    return false;
}

bool Device::controlPacketReceived(NodeID_t target,
                                   CtlReq_t req,
                                   const ControlPacket & packet)
{
    if(isNodeExists(target))
        return getNode(target)->controlPacketReceived(req, packet);

    return false;
}

void Device::init()
{
	for(auto n : _nodes)
		n->init();
}

bool Device::deviceDescriptorRequested(ControlPacket & packet)
{
    return DeviceDescriptor(packet).init(_nodes.size(),
                                         _maxCtlReqSize);
}
