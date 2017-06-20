#include "qistreamcontroller.h"
#include <string.h>
#include <qcore/controller/qrwpropertycontroller.h>
#include <QDebug>

////////////////////////////////////////////////////////////////////////////////

QIStreamController::QIStreamController(NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * connection) :
    QNodeController (node_id, parent_id, name, connection),
    _enabled(nullptr)
{}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::isStreamEnabled() const
{
    if(_enabled == nullptr)
        return false;
    return _enabled->get().toBool();
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::setStreamEnabled(bool enabled)
{
    if(_enabled == nullptr)
        return false;
    return _enabled->set(enabled);
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::addChannel(QIStreamChannelController *channel) {
    if(channel->dataOffset() + channel->dataLength() > packetSize())
        return false;
    _channels.push_back(channel);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::eventInit(DeviceController * controller)
{
    _enabled = findChild<QRWPropertyController*>("Enabled",
                                                 Qt::FindDirectChildrenOnly);
    if(_enabled == nullptr)
        return false;

    connect(_enabled, &QRWPropertyController::valueChanged,
            [this](const QVariant & value)
            {
                eventStreamToggled(value.toBool());
            });

    return QNodeController::eventInit(controller);
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::eventData(const ControlPacket & packet)
{
    (void)packet;
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QIStreamController::processChannels(const ArrayRef<Data> & buffer)
{
    if(!isStreamEnabled())
        return;

    Data * ptr = buffer.data();

    for(QIStreamChannelController * channel : _channels)
    {
        ArrayRef<Data> arr(ptr + channel->dataOffset(), channel->dataLength());
        channel->fill(arr);
    }
}
