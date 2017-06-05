#include "qostreamcontroller.h"
#include <qcore/controller/forms/ostreamform.h>

QOStreamController::QOStreamController(NodeID_t node_id,
                                       NodeID_t parent_id,
                                       const QString & name,
                                       QDeviceConnection * conn) :
    QNodeController (node_id, parent_id, name, conn),
    _enabled(false)
{

}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::setStreamEnabled(bool enabled)
{
    if(enabled == _enabled)
        return true;

    DataPacket<bool> en_packet(beginCtlTransfer());
    if(!en_packet.init(enabled))
    {
        logMessage("setStreamEnabled : unable to initialize ctl transfer");
        abortCtlTransfer();
        return false;
    }
    return submitCtlTransfer(en_packet, [this](){ readData(); });
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
        if(channel->dataOffset() + channel->dataLength() < buffer.size())
            channel->streamDataReceived(ptr_base + channel->dataOffset());
        else
            logMessage("OStream buffer overflow");
}

////////////////////////////////////////////////////////////////////////////////

NodeControllerForm * QOStreamController::createForm(QWidget * parent)
{
    return new OStreamForm(this, parent);
}

////////////////////////////////////////////////////////////////////////////////

bool QOStreamController::eventData(const ControlPacket & packet)
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
