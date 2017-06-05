#ifndef DEVICE_H
#define DEVICE_H
#include <core/common/types.h>

class Node;
class ControlPacket;
class IStreamNode;
class OStreamNode;

class Device
{
public:
    static const uint8_t MAX_STREAM_COUNT = 1;

    explicit Device(Length_t max_ctl_req_size);

    void addNode(Node * node);

    bool controlPacketRequested(NodeID_t target,
                                CtlReq_t req,
                                ControlPacket & packet);

    bool controlPacketReceived(NodeID_t target,
                               CtlReq_t req,
                               const ControlPacket & packet);

    void init();

private:
    bool deviceInfoRequested(ControlPacket & packet);

    bool deviceDescriptorRequested(ControlPacket & packet);

    inline bool isNodeExists(NodeID_t id) const { return id < _nodes.size(); }

    inline Node * getNode(NodeID_t id) const { return _nodes[id]; }

    std::vector<Node*>        _nodes;
    std::vector<IStreamNode*> _inputStreams;
    std::vector<OStreamNode*> _outputStreams;
    Length_t                  _maxCtlReqSize;
};

#endif //DEVICE_H
