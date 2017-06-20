#include "qoparamcontroller.h"
#include <core/common/defaultnodes.h>

QOParamController::QOParamController(NodeType_t type,
                                     NodeID_t node_id,
                                     NodeID_t parent_id,
                                     const QString & name,
                                     QDeviceConnection * dev) :
    QOStreamChannelController (node_id, parent_id, name, dev),
    _type(type)
{

}

QNodeController * QOParamController::createInstance(NodeID_t node_id,
                                                    NodeID_t parent_id,
                                                    const QString & name,
                                                    QDeviceConnection * device)
{
    return new QOParamController(_type, node_id, parent_id,
                                 name, device);
}

void QOParamController::streamDataReceived(Data * data)
{
    switch(_type)
    {
    case NODE_TYPE_INT8_OPARAM :
    {
        ParamData<int8_t> * pdata = reinterpret_cast<ParamData<int8_t>*>(data);
        if(pdata->id == _lastId)
            return;
        _lastId = pdata->id;
        _value = QVariant(pdata->value);
        emit valueChanged(_value);
        return;
    }

    case NODE_TYPE_INT16_OPARAM :
    {
        ParamData<int16_t> * pdata = reinterpret_cast<ParamData<int16_t>*>(data);
        if(pdata->id == _lastId)
            return;
        _lastId = pdata->id;
        _value = QVariant(pdata->value);
        emit valueChanged(_value);
        return;
    }

    case NODE_TYPE_INT32_OPARAM :
    {
        ParamData<int32_t> * pdata = reinterpret_cast<ParamData<int32_t>*>(data);
        if(pdata->id == _lastId)
            return;
        _lastId = pdata->id;
        _value = QVariant(pdata->value);
        emit valueChanged(_value);
        return;
    }

    case NODE_TYPE_FLOAT32_OPARAM :
    {
        ParamData<float> * pdata = reinterpret_cast<ParamData<float>*>(data);
        if(pdata->id == _lastId)
            return;
        _lastId = pdata->id;
        _value = QVariant(pdata->value);
        emit valueChanged(_value);
        return;
    }

    case NODE_TYPE_FLOAT64_OPARAM :
    {
        ParamData<double> * pdata = reinterpret_cast<ParamData<double>*>(data);
        if(pdata->id == _lastId)
            return;
        _lastId = pdata->id;
        _value = QVariant(pdata->value);
        emit valueChanged(_value);
        return;
    }
    }
}
