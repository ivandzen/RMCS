#include "qropropertycontroller.h"
#include <core/common/defaultnodes.h>
#include <qcore/controller/forms/qboolropropertyform.h>
#include <qcore/controller/forms/qintropropertyform.h>
#include <qcore/controller/forms/qfloatropropertyform.h>

QROPropertyController::QROPropertyController(NodeType_t type,
                                             NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * dev) :
    QNodeController (node_id, parent_id, name, dev),
    _type(type)
{

}

QNodeController * QROPropertyController::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceConnection * device)
{
    return new QROPropertyController(_type, node_id, parent_id, name, device);
}

NodeControllerForm * QROPropertyController::createForm(QWidget * parent)
{
    switch(_type)
    {
    case NODE_TYPE_BOOL_PROP_RDONLY :
        return new QBoolROPropertyForm(this, parent);

    case NODE_TYPE_INT_PROP_RDONLY :
        return new QIntROPropertyForm(this, parent);

    case NODE_TYPE_FLOAT_PROP_RDONLY :
        return new QFloatROPropertyForm(this, parent);

    default : break;
    }

    return nullptr;
}

NodeType_t QROPropertyController::type() const
{
    return _type;
}

bool QROPropertyController::eventSetup(const ControlPacket &)
{
    return true;
}

bool QROPropertyController::eventData(const ControlPacket & packet)
{
    switch(_type)
    {
    case NODE_TYPE_BOOL_PROP_RDONLY :
    {
        DataPacket<bool> data(packet);
        if(!data.isValid())
            return false;
        _value = QVariant(*data.get());
        emit updated();
        return true;
    }

    case NODE_TYPE_INT_PROP_RDONLY :
    {
        DataPacket<int32_t> data(packet);
        if(!data.isValid())
            return false;
        _value = QVariant(*data.get());
        emit updated();
        return true;
    }

    case NODE_TYPE_FLOAT_PROP_RDONLY :
    {
        DataPacket<float> data(packet);
        if(!data.isValid())
            return false;
        _value = QVariant(*data.get());
        emit updated();
        return true;
    }

    default : break;
    }

    return false;
}
