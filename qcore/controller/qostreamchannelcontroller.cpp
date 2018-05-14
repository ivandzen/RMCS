#include "qostreamchannelcontroller.h"
#include <qcore/controller/qostreamcontroller.h>
#include <qcore/controller/qdevicecontroller.h>
#include <QMutexLocker>

QOStreamChannelController::QOStreamChannelController(NodeID_t node_id,
                                                     NodeID_t parent_id,
                                                     const QString & name,
                                                     QDeviceConnection * dev) :
    QNodeController(node_id, parent_id, name, dev)
{

}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamChannelController::setEnabled(bool enabled)
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
        else logMessage("setEnabled control transfer not completed");
    });
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamChannelController::eventSetup(const ControlPacket & packet)
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

bool QOStreamChannelController::eventData(const ControlPacket &packet)
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

bool QOStreamChannelController::eventInit(DeviceController * dev)
{
    QDeviceController * qdev = (QDeviceController*)dev; //! @attention

    QOStreamController * stream =
            qobject_cast<QOStreamController*>(qdev->getNode(_streamId));

    if((stream == nullptr) || !stream->addChannel(this))
        return false;

    return QNodeController::eventInit(dev);
}
