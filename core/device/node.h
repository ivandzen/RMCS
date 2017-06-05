#ifndef NODE_H
#define NODE_H
#include <core/common/types.h>

class Device;
class ControlPacket;
class InputStream;

class Node
{
    friend class Device;
public:
    Node(NodeType_t type,
         const char * name,
         Node * parent);

    Node(NodeType_t type,
         const char * name,
         Device * device);

    virtual ~Node();

    inline NodeID_t id() const { return _id; }

    inline NodeType_t type() const { return _type; }

    inline const char * name() const { return _name; }

    inline Node * parent() const { return _parent; }

    inline Device * device() const { return _device; }

    inline NodeStatus_t nodeStatus() const { return _status; }

protected:
    virtual bool settingsRequested(ControlPacket & packet) const = 0;

    virtual bool nodeDataRequested(ControlPacket &) const = 0;

    virtual bool nodeDataReceived(const ControlPacket &) = 0;

    virtual bool init();

    virtual void reset();

    void setStatus(NodeStatus st) { _status = st; }

    virtual void printLog(const char *) { /** @todo **/ }

private:
    bool controlPacketRequested(CtlReq_t req, ControlPacket & packet);

    bool controlPacketReceived(CtlReq_t req, const ControlPacket & packet);

    bool descriptorRequested(ControlPacket & packet) const;

    bool nodeStatusRequested(ControlPacket & packet) const;

    NodeID_t            _id;
    NodeType_t          _type;
    const char * const  _name;
    Node *              _parent;
    Device *            _device;
    NodeStatus_t        _status;
};

#endif //NODE_H
