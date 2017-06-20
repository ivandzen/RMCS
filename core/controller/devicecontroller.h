#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H
#include <core/common/arrayref.h>
#include <core/controller/deviceconnection.h>

class NodeController;
class IStreamController;

class DeviceController
{
    friend class DeviceConnection;
public:
    enum DeviceState
    {
        STATE_UNDEFINED,
        STATE_INIT,
        STATE_WORK,
        STATE_ERROR,
        STATE_RESTART
    };

    typedef DeviceConnection::ReceiveHandler     ReceiveHandler;
    typedef DeviceConnection::TransmitHandler    TransmitHandler;

    explicit DeviceController(DeviceConnection * connection);

    virtual ~DeviceController();

    inline DeviceConnection * connection() const {
        return _connection;
    }

    bool init();

    bool fullUpdate();

    bool requestNodeControlPacket(NodeID_t node_id,
                                  CtlReq_t type,
                                  const ReceiveHandler & handler);

    bool requestDeviceControlPacket(CtlReq_t type,
                                    const ReceiveHandler & handler);

    ControlPacket beginCtlTransfer();

    void abortCtlTransfer();

    bool endNodeControlTransaction(NodeController *ep,
                                   CtlReq_t type,
                                   const ControlPacket & packet,
                                   const TransmitHandler & handler);

    bool endControlDevTransaction(CtlReq_t type,
                                  const ControlPacket & packet,
                                  const TransmitHandler & handler);

    inline DeviceState state() const { return _state; }

    inline size_t numNodes() const
    {
        return _nodes.size();
    }

    inline bool isNodeExists(NodeID_t node_id) const
    {
        return node_id < _nodes.size();
    }

    inline NodeController * getNode(NodeID_t node_id) const
    {
        return _nodes[node_id];
    }

protected:
    virtual void restartedEvent() {}

    virtual void updatedEvent() {}

    virtual void beforeInitNodes() {}

    virtual void afterInitNodes() {}

    virtual void errorEvent() {}

    virtual void logMessage(const char * message) = 0;

    virtual NodeController * createNode(NodeType_t node_type,
                                        NodeID_t node_id,
                                        NodeID_t parent_id,
                                        const ArrayRef<char> & name) = 0;

    ///////////////////////////////////////////////////////////////////////
    /// Backend event handlers

    void backendEventError();

    void backendEventTimeout();

    void backendEventConnLost();

    void backendEventRestart();

private:
    ///////////////////////////////////////////////////////////////////////

    void parseDeviceDescriptor(const ControlPacket & packet);

    void parseNodeDescriptor(const ControlPacket & packet);

    void parseNodeSettings(const ControlPacket & packet);

    void parseNodeData(const ControlPacket & packet);

    void error(const char * message);

    void warning(const char * message);

    ///////////////////////////////////////////////////////////////////////
    /// Node related types definintions and endpoints operations
    std::vector<NodeController*>    _nodes;
    DeviceState                     _state;
    NodeID_t                        _currentNode;
    Length_t                        _maxCtlPackLength;

    ///////////////////////////////////////////////////////////////////////
    /// Target application dependent logic
    ///
    DeviceConnection *   _connection;
};

#endif // DEVICECONTROLLER_H
