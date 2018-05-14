#ifndef DEVICECONNECTION_H
#define DEVICECONNECTION_H
#include <core/common/types.h>
#include <core/common/arrayref.h>

class ControlPacket;
class DeviceController;

class DeviceConnection
{
public:
    typedef std::function<void(bool)> TransmitHandler;
    typedef std::function<void(const ControlPacket &, bool)> ReceiveHandler;

    DeviceConnection() :
        _controller(nullptr) {}

    inline bool setController(DeviceController * controller)
    {
        if(_controller != nullptr)
            return false;
        _controller = controller;
        return true;
    }

    inline DeviceController * controller() const { return _controller; }

    virtual ~DeviceConnection() {}

    virtual ControlPacket beginCtlTransfer(Length_t max_length) = 0;

    virtual void abortCtlTransfer() = 0;

    virtual bool endCtlTransfer(NodeID_t node_id,
                                CtlReq_t type,
                                const ControlPacket & packet,
                                const TransmitHandler & handler) = 0;

    virtual bool requestCtlPacket(NodeID_t node_id,
                                  CtlReq_t type,
                                  Length_t max_length,
                                  const ReceiveHandler & handler) = 0;

protected:
    //void errorEvent() const;

    //void timeoutEvent() const;

    //void disconnectEvent() const;

    //void restartEvent() const;

private:
    DeviceController * _controller;
};

#endif // DEVICECONNECTION_H
