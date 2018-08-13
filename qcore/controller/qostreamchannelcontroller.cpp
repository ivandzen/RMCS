#include "qostreamchannelcontroller.h"
#include <qcore/controller/qostreamcontroller.h>
#include <qcore/controller/qdevicecontroller.h>
#include <QMutexLocker>

QOStreamChannelController::QOStreamChannelController(NodeID_t node_id,
                                                     NodeID_t parent_id,
                                                     const QString & name,
                                                     QDeviceConnection * dev) :
    QNodeController(node_id, parent_id, name, dev),
    _streamId(NodeIDNull),
    _dataOffset(0),
    _dataLength(0),
    _count(0)
{

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

bool QOStreamChannelController::eventInit(DeviceController * dev)
{
    QDeviceController * qdev = (QDeviceController*)dev; //! @attention

    QOStreamController * stream =
            qobject_cast<QOStreamController*>(qdev->getNode(_streamId));

    if((stream == nullptr) || !stream->addChannel(this))
        return false;

    return QNodeController::eventInit(dev);
}

bool QOStreamChannelController::eventData(const ControlPacket &packet)
{
    if(_count == 0)
    {
        DataPacket<ParamDataIdx> countp(packet);
        if(!countp.isValid())
        {
            logMessage("Invalid count packet");
            return false;
        }
        _count = *countp.get();
    }

    return true;
}
