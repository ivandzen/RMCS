#include "qistreamcontroller.h"
#include <string.h>
#include <qcore/controller/forms/istreamform.h>

////////////////////////////////////////////////////////////////////////////////

QIStreamController::QIStreamController(NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * connection) :
    QNodeController (node_id, parent_id, name, connection),
    _enabled(false)
{}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::setStreamEnabled(bool enabled)
{
    if(_enabled == enabled)
        return true;
    DataPacket<bool> packet(beginCtlTransfer());
    if(!packet.init(enabled))
    {
        logMessage("setStreamEnabled : unable to initialize ctl transfer");
        abortCtlTransfer();
        return false;
    }
    return submitCtlTransfer(packet, [this](){ readData(); });
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::addChannel(QIStreamChannelController *channel) {
    if(channel->dataOffset() + channel->dataLength() >= packetSize())
        return false;
    _channels.push_back(channel);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

NodeControllerForm *QIStreamController::createForm(QWidget *parent)
{
    return new IStreamForm(this, parent);
}

////////////////////////////////////////////////////////////////////////////////

bool QIStreamController::eventData(const ControlPacket & packet)
{
    DataPacket<bool> data(packet);
    if(!data.isValid())
        return false;
    bool enable_stream = *data.get();
    if(enable_stream != _enabled)
    {
        _enabled = enable_stream;
        eventStreamToggled(_enabled);
        emit streamToggled(_enabled);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QIStreamController::processChannels(const ArrayRef<Data> & buffer)
{
    if(!isStreamEnabled())
        return;

    for(auto channel : _channels)
    {
        Data * channel_data = channel->data();
        if(channel_data)
            memcpy(buffer.data() + channel->dataOffset(),
                   channel_data,
                   channel->dataLength());
        else
            logMessage("channel controller return null pointer");
    }
}
