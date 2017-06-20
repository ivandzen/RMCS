#include "qostreamcontroller.h"
#include <qcore/controller/qrwpropertycontroller.h>

QOStreamController::QOStreamController(NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * conn) :
    QNodeController (node_id, parent_id, name, conn),
    _enabled(nullptr)
{

}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::isStreamEnabled() const
{
    if(_enabled == nullptr)
        return false;
    return _enabled->get().toBool();
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::setStreamEnabled(bool enabled)
{
    if(_enabled == nullptr)
        return false;
    return _enabled->set(enabled);
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::addChannel(QOStreamChannelController * channel)
{
    _channels.push_back(channel);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QOStreamController::processChannels(const ArrayRef<Data> & buffer)
{
    if(!isStreamEnabled())
        return;

    Data * ptr_base = buffer.data();
    for(auto channel : _channels)
        if(channel->dataOffset() + channel->dataLength() <= buffer.size())
            channel->streamDataReceived(ptr_base + channel->dataOffset());
        else
            logMessage("OStream buffer overflow");
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::eventInit(DeviceController *device)
{
    _enabled = findChild<QRWPropertyController*>("Enabled", Qt::FindDirectChildrenOnly);
    if(_enabled == nullptr)
        return false;

    connect(_enabled, &QRWPropertyController::valueChanged,
            [this](const QVariant & value)
            {
                eventStreamToggled(value.toBool());
            });

    return QNodeController::eventInit(device);
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::eventData(const ControlPacket & packet)
{
    (void)packet;
    return true;
}
