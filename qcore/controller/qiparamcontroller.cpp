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

void QIParamController::setValue(const QVariant & value)
{
    if(!value.isValid() || (value == _value))
        return;
    _value = value;
    ++_dataId;
    _ready = true;
}

void QIParamController::fill(ArrayRef<Data> & arr)
{
    if(!_ready)
        return;
    _ready = false;

    switch(_type)
    {
    case NODE_TYPE_INT8_IPARAM :
    {
        if(!_value.canConvert<int8_t>())
            return;
        ParamData<int8_t> data;
        data.id = _dataId;
        data.value = _value.value<int8_t>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_INT16_IPARAM :
    {
        if(!_value.canConvert<int16_t>())
            return;
        ParamData<int16_t> data;
        data.id = _dataId;
        data.value = _value.value<int16_t>();
        arr.copyFrom(reinterpret_cast<Data*>(&data));
        return;
    }

    case NODE_TYPE_INT32_IPARAM :
    {
        if(!_value.canConvert<int32_t>())
            return;
        ParamData<int32_t> data;
        data.id = _dataId;
        data.value = _value.value<int32_t>();
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
