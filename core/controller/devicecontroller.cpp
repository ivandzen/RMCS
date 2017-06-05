#include "devicecontroller.h"
#include <common/control_protocol.h>
#include "nodecontroller.h"

#define MAX_CTL_PACK_LEN 2048

///////////////////////////////////////////////////////////////////////////////////////

DeviceController::DeviceController(DeviceConnection * connection) :
    _state(STATE_UNDEFINED),
    _maxCtlPackLength(1024),
    _connection(connection)
{
    _connection->setController(this);
}

///////////////////////////////////////////////////////////////////////////////////////

DeviceController::~DeviceController()
{
    for(NodeController * node : _nodes)
        node->eventDestroy();
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::init()
{
    if(_state != STATE_UNDEFINED)   //! возможно еще какие то состояния допустимы
        return false;
    _state = STATE_INIT;
    return requestDeviceControlPacket(CTLREQ_DEV_DESCRIPTOR,
                                      [this](const ControlPacket & packet)
                                            { parseDeviceDescriptor(packet); });
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::fullUpdate()
{
    if(_state != STATE_WORK)
    {
        warning("Full update may only be performed in WORK state");
        return false;
    }
    _currentNode = 0;
    if(requestNodeControlPacket(_currentNode,
                                CTLREQ_NODE_DATA,
                                [this](const ControlPacket & packet)
                                      { parseNodeData(packet); })) return true;
    warning("Unable to request first Node data");
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::requestNodeControlPacket(NodeID_t node_id,
                                                CtlReq_t type,
                                                const ReceiveHandler &handler)
{
    return _connection->requestCtlPacket(node_id,
                                         type,
                                         _maxCtlPackLength,
                                         handler);
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::requestDeviceControlPacket(CtlReq_t type,
                                                      const ReceiveHandler & handler)
{
    return _connection->requestCtlPacket(NodeIDNull,
                                         type,
                                         _maxCtlPackLength,
                                         handler);
}

///////////////////////////////////////////////////////////////////////////////////////

ControlPacket DeviceController::beginCtlTransfer()
{
    return _connection->beginCtlTransfer(_maxCtlPackLength);
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::abortCtlTransfer()
{
    _connection->abortCtlTransfer();
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::endNodeControlTransaction(NodeController *ep,
                                                 CtlReq_t type,
                                                 const ControlPacket & packet,
                                                 const TransmitHandler & handler)
{
    return _connection->endCtlTransfer(ep->id(),
                                       type,
                                       packet,
                                       handler);
}

///////////////////////////////////////////////////////////////////////////////////////

bool DeviceController::endControlDevTransaction(CtlReq_t type,
                                                    const ControlPacket & packet,
                                                    const TransmitHandler & handler)
{
    return _connection->endCtlTransfer(NodeIDNull,
                                       type,
                                       packet,
                                       handler);
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::backendEventError()
{

}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::backendEventTimeout()
{

}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::backendEventConnLost()
{

}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::backendEventRestart()
{
    restartedEvent();
    init();
}

///////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>

void DeviceController::parseDeviceDescriptor(const ControlPacket & packet)
{
    DeviceDescriptor descriptor(packet);
    if(!descriptor.isValid())
    {
        error("Invalid device descriptor");
        return;
    }
    _nodes.resize(descriptor.numNodes());
    _maxCtlPackLength = descriptor.maxCtlPackLength();

    _currentNode = 0;
    if(_currentNode == _nodes.size())
    {
        warning("Device contains no nodes");
        _state = STATE_WORK;
        return;
    }

    if(requestNodeControlPacket(_currentNode,
                                CTLREQ_NODE_DESCRIPTOR,
                                [this](const ControlPacket & packet)
                                      { parseNodeDescriptor(packet); })) return;

    error("Unable to request Node descriptor");
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::parseNodeDescriptor(const ControlPacket & packet)
{
    NodeDescriptor descriptor(packet);
    if(!descriptor.isValid())
    {
        error("Wrong Node descriptor");
        return;
    }

    if(NodeController * controller = createNode(descriptor.nodeType(),
                                                descriptor.id(),
                                                descriptor.parent_id(),
                                                ArrayRef<char>(descriptor.name(),
                                                               descriptor.nameLength())))
    {
        _nodes[_currentNode] = controller;
        ++_currentNode;
        if(_currentNode == _nodes.size())
        {
            _currentNode = 0;
            if(requestNodeControlPacket(_currentNode,
                                        CTLREQ_NODE_SETTINGS,
                                        [this](const ControlPacket & packet)
                                              { parseNodeSettings(packet); })) return;
            error("Node Custom descriptor not requested");
            return;
        }
        else if(requestNodeControlPacket(_currentNode,
                                         CTLREQ_NODE_DESCRIPTOR,
                                         [this](const ControlPacket & packet)
                                               { parseNodeDescriptor(packet); })) return;

        error("Next Node descriptor not requested");
        return;
    }

    error("Node controller not created.");
    return;
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::parseNodeSettings(const ControlPacket & packet)
{
    if(!_nodes[_currentNode]->eventSetup(packet))
        warning("Unable to parse Node settings");

    ++_currentNode;
    if(_currentNode == _nodes.size())
    {
        for(_currentNode = 0;
            _currentNode != _nodes.size();
            ++_currentNode)
            _nodes[_currentNode]->init(this);

        _state = STATE_WORK;
        initializedEvent();
        return;
    }
    if(requestNodeControlPacket(_currentNode,
                                CTLREQ_NODE_SETTINGS,
                                [this](const ControlPacket & packet)
                                      { parseNodeSettings(packet); })) return;
    error("Unable to request next Node settings");
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::parseNodeData(const ControlPacket & packet)
{
    if(!_nodes[_currentNode]->eventData(packet))
        error("Unable to parse Node data");

    ++_currentNode;
    if(_currentNode == _nodes.size())
    {
        _state = STATE_WORK;
        updatedEvent();
        return;
    }
    if(requestNodeControlPacket(_currentNode,
                                CTLREQ_NODE_DATA,
                                [this](const ControlPacket & packet)
                                      { parseNodeData(packet); })) return;
    error("Unable to request next Node data");
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::error(const char * message)
{
    (void)message;
    //! @todo
    _state = STATE_ERROR;
    logMessage(message);
    errorEvent();
    //log(message);
}

///////////////////////////////////////////////////////////////////////////////////////

void DeviceController::warning(const char *message)
{
    (void)message;
    logMessage(message);
}
