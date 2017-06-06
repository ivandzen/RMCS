#include "qiparamcontroller.h"
#include <core/common/defaultnodes.h>

QIParamController::QIParamController(NodeType_t type,
                                     NodeID_t node_id,
                                     NodeID_t parent_id,
                                     const QString & name,
                                     QDeviceConnection * connection) :
    QIStreamChannelController (node_id, parent_id, name, connection),
    _type(type)
{

}

QNodeController *QIParamController::createInstance(NodeID_t node_id,
                                                   NodeID_t parent_id,
                                                   const QString & name,
                                                   QDeviceConnection * device)
{
    return new QIParamController(_type, node_id, parent_id, name, device);
}

NodeControllerForm *QIParamController::createForm(QWidget * parent)
{

}

void QIParamController::setValue(const QVariant & value)
{
    if(!value.isValid() || (value == _value))
        return;
    _value = value;
    _ready = true;
}

void QIParamController::fill(ArrayRef<Data> & arr) const
{
    if(!_ready)
        return;

    switch(_type)
    {
    case NODE_TYPE_UINT8_IPARAM :
    {
        if(!_value.canConvert<uint8_t>())
            return;
        ParamData<uint8_t> data;
        data.id = _dataId;
        data.value = _value.value<uint8_t>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_UINT16_IPARAM :
    {
        if(!_value.canConvert<uint16_t>())
            return;
        ParamData<uint16_t> data;
        data.id = _dataId;
        data.value = _value.value<uint16_t>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_UINT32_IPARAM :
    {
        if(!_value.canConvert<uint32_t>())
            return;
        ParamData<uint32_t> data;
        data.id = _dataId;
        data.value = _value.value<uint32_t>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_FLOAT32_IPARAM :
    {
        if(!_value.canConvert<float>())
            return;
        ParamData<float> data;
        data.id = _dataId;
        data.value = _value.value<float>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_FLOAT64_IPARAM :
    {
        if(!_value.canConvert<double>())
            return;
        ParamData<double> data;
        data.id = _dataId;
        data.value = _value.value<double>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }
    }
}
