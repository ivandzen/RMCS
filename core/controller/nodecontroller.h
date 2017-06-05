#ifndef NODECONTROLLER_H
#define NODECONTROLLER_H
#include <core/common/types.h>
#include <core/common/control_protocol.h>
#include <core/controller/deviceconnection.h>

class ControlPacket;
class DeviceController;

class NodeController
{
    friend class DeviceController;
public:
    typedef DeviceConnection::TransmitHandler TransmitHandler;

    NodeController(NodeID_t id,
                   NodeID_t parent_id,
                   const std::string & name,
                   DeviceConnection * connection);

    virtual ~NodeController();

    inline NodeID_t id() const { return _id; }

    inline NodeID_t parentId() const { return _parentId; }

    inline const std::string & name() const { return _name; }

    inline NodeStatus_t status() const { return _status; }

    bool readStatus();

    bool readData();

    inline bool isInitialized() const {
        return _initialized;
    }

protected:
    ControlPacket beginCtlTransfer();

    bool submitCtlTransfer(const ControlPacket & packet,
                           const TransmitHandler & handler);

    void abortCtlTransfer();

    virtual void eventDestroy() = 0;

    virtual bool eventSetup(const ControlPacket & settings) = 0;

    virtual bool eventData(const ControlPacket & packet) = 0;

    virtual void eventStatus(NodeStatus_t status) = 0;

    virtual bool eventInit(DeviceController * controller) = 0;

    virtual void logMessage(const char * message) = 0;

private:
    void statusReceived(const ControlPacket &  packet);

    void init(DeviceController * controller);

    NodeID_t            _id;
    NodeID_t            _parentId;
    std::string         _name;
    NodeStatus_t        _status;

    DeviceConnection *  _connection;
    bool                _initialized;
};

#endif // NODECONTROLLER_H
