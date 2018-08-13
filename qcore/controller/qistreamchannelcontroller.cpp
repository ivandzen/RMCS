#include "qistreamchannelcontroller.h"
#include <core/common/control_protocol.h>
#include <qcore/controller/qistreamcontroller.h>
#include <qcore/controller/qdevicecontroller.h>

QIStreamChannelController::QIStreamChannelController(NodeID_t node_id,
                                                     NodeID_t parent_id,
                                                     const QString & name,
                                                     QDeviceConnection * connection) :
    QNodeController(node_id, parent_id, name, connection)
{

}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamChannelController::setEnabled(bool enabled)
{
    if(enabled == _enabled)
        return true;

    DataPacket<bool> en_packet(beginCtlTransfer());
    if(!en_packet.init(enabled))
    {
        logMessage("Error while initializing ctl transfer");
        abortCtlTransfer();
        return false;
    }
    return submitCtlTransfer(en_packet,
                             [this](bool success)
    {
        if(success) readData();
        else logMessage("SetEnabled control transfer not completed");
    });
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamChannelController::eventSetup(const ControlPacket & packet)
{
    StreamChannelSettings settings(packet);
    if(!settings.isValid())
        return false;

    _streamId = settings.streamID();
    _dataOffset = settings.dataOffset();
    _dataLength = settings.dataLength();
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamChannelController::eventData(const ControlPacket & packet)
{
    DataPacket<bool> en_packet(packet);
    if(!en_packet.isValid())
        return false;

    if(_enabled != *en_packet.get())
    {
        _enabled = *en_packet.get();
        emit toggled(_enabled);
        toggledEvent(_enabled);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamChannelController::eventInit(DeviceController * controller)
{
    QDeviceController * qdev = (QDeviceController*)controller;

    QIStreamController * stream =
            qobject_cast<QIStreamController*>(qdev->getNode(_streamId));

    if((stream == nullptr) || !stream->addChannel(this))
        return false;
    return QNodeController::eventInit(controller);
}
