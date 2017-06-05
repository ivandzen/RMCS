#include "nodecontroller.h"
#include "devicecontroller.h"

NodeController::NodeController(NodeID_t id,
                               NodeID_t parent_id,
                               const std::string & name,
                               DeviceConnection * connection) :
    _id(id),
    _parentId(parent_id),
    _name(name),
    _connection(connection),
    _initialized(false)
{
    //! @todo
}

NodeController::~NodeController()
{

}

bool NodeController::readStatus()
{
    if(!_initialized) return false;
    return _connection->requestCtlPacket(id(), CTLREQ_NODE_STATUS, 2048, //! @todo заменить на константу
                                         [this](const ControlPacket & packet)
                                               { statusReceived(packet); });
}

bool NodeController::readData()
{
    if(!_initialized) return false;
    return _connection->requestCtlPacket(id(),
                                         CTLREQ_NODE_DATA, 2048, //! @todo заменить на константу
                                         [this](const ControlPacket & packet)
                                               { (void)eventData(packet); });
}

ControlPacket NodeController::beginCtlTransfer()
{
    return _connection->beginCtlTransfer(2048);
}

bool NodeController::submitCtlTransfer(const ControlPacket & packet,
                                       const TransmitHandler & handler)
{
    return _connection->endCtlTransfer(id(), CTLREQ_NODE_DATA, packet, handler);
}

void NodeController::abortCtlTransfer()
{
    return _connection->abortCtlTransfer();
}

void NodeController::statusReceived(const ControlPacket & packet)
{
    DataPacket<NodeStatus_t> status(packet);
    if(!status.isValid())
    {
        logMessage("Invalid status packet");
        return;
    }
    if(_status != *status.get())
    {
        _status = *status.get();
        eventStatus(_status);
    }
}

void NodeController::init(DeviceController * controller)
{
    _initialized = eventInit(controller);
}

