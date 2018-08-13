#include "qrwpropertycontroller.h"
#include <core/common/defaultnodes.h>

QRWPropertyController::QRWPropertyController(NodeType_t node_type,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * dev) :
    QROPropertyController (node_type, node_id, parent_id, name, dev)
{

}

QNodeController *QRWPropertyController::createInstance(NodeID_t node_id,
                                                       NodeID_t parent_id,
                                                       const QString & name,
                                                       QDeviceConnection * device)
{
    return new QRWPropertyController(type(), node_id, parent_id, name, device);
}

bool QRWPropertyController::set(const QVariant & value)
{
    switch(type())
    {
    case NODE_TYPE_BOOL_PROP :
    {
        if(!value.canConvert<bool>())
            return false;
        if(get() == value)
            return true;

        DataPacket<bool> packet(beginCtlTransfer());
        if(!packet.init(value.toBool()))
        {
            abortCtlTransfer();
            return false;
        }

        return submitCtlTransfer(packet, [this](bool success)
        {
            if(success) update();
            else logMessage("Unable to set property value. Control transfer not completed");
        });
    }

    case NODE_TYPE_INT_PROP :
    {
        if(!value.canConvert<uint32_t>())
            return false;
        if(get() == value)
            return true;

        DataPacket<uint32_t> packet(beginCtlTransfer());
        if(!packet.init(value.toInt()))
        {
            abortCtlTransfer();
            return false;
        }

        return submitCtlTransfer(packet, [this](bool success)
        {
            if(success) update();
            else logMessage("Unable to set property value. Control transfer not completed");
        });
    }

    case NODE_TYPE_FLOAT_PROP :
    {
        if(!value.canConvert<float>())
            return false;
        if(get() == value)
            return true;

        DataPacket<float> packet(beginCtlTransfer());
        if(!packet.init(value.toFloat()))
        {
            abortCtlTransfer();
            return false;
        }

        return submitCtlTransfer(packet, [this](bool success)
        {
            if(success) update();
            else logMessage("Unable to set property value. Control transfer not completed");
        });
    }

    default : break;
    }
    return false;
}

bool QRWPropertyController::eventData(const ControlPacket & packet)
{
    switch(type())
    {
    case NODE_TYPE_BOOL_PROP :
    {
        DataPacket<bool> data(packet);
        if(!data.isValid())
            return false;
        setValue(*data.get());
        return true;
    }

    case NODE_TYPE_INT_PROP :
    {
        DataPacket<int32_t> data(packet);
        if(!data.isValid())
            return false;
        setValue(*data.get());
        return true;
    }

    case NODE_TYPE_FLOAT_PROP :
    {
        DataPacket<float> data(packet);
        if(!data.isValid())
            return false;
        setValue(*data.get());
        return true;
    }

    default : break;
    }

    return false;
}
