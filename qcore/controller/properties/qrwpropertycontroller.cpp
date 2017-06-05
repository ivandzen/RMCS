#include "qrwpropertycontroller.h"
#include <core/common/defaultnodes.h>
#include <qcore/controller/forms/qboolrwpropertyform.h>
#include <qcore/controller/forms/qintrwpropertyform.h>
#include <qcore/controller/forms/qfloatrwpropertyform.h>

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

NodeControllerForm *QRWPropertyController::createForm(QWidget * parent)
{
    switch(type())
    {
    case NODE_TYPE_BOOL_PROP :
        return new QBoolRWPropertyForm(this, parent);

    case NODE_TYPE_INT_PROP :
        return new QIntRWPropertyForm(this, parent);

    case NODE_TYPE_FLOAT_PROP :
        return new QFloatRWPropertyForm(this, parent);

    default : break;
    }

    return nullptr;
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

        return submitCtlTransfer(packet, [this](){ update(); });
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

        return submitCtlTransfer(packet, [this](){ update(); });
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

        return submitCtlTransfer(packet, [this](){ update(); });
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
        emit updated();
        return true;
    }

    case NODE_TYPE_INT_PROP :
    {
        DataPacket<int32_t> data(packet);
        if(!data.isValid())
            return false;
        setValue(*data.get());
        emit updated();
        return true;
    }

    case NODE_TYPE_FLOAT_PROP :
    {
        DataPacket<float> data(packet);
        if(!data.isValid())
            return false;
        setValue(*data.get());
        emit updated();
        return true;
    }

    default : break;
    }

    return false;
}
