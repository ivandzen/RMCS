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

NodeControllerForm * QOParamController::createForm(QWidget * parent)
{

}

void QOParamController::streamDataReceived(Data * data)
{
    switch(_type)
    {
    case NODE_TYPE_UINT8_OPARAM :
    {
        ParamData<uint8_t> * pdata = reinterpret_cast<ParamData<uint8_t>*>(data);
        if(pdata->id == _lastId)
            return;
        emit valueChanged(pdata->value);
        _lastId = pdata->id;
        return;
    }

    case NODE_TYPE_UINT16_OPARAM :
    {
        ParamData<uint16_t> * pdata = reinterpret_cast<ParamData<uint16_t>*>(data);
        if(pdata->id == _lastId)
            return;
        emit valueChanged(pdata->value);
        _lastId = pdata->id;
        return;
    }

    case NODE_TYPE_UINT32_OPARAM :
    {
        ParamData<uint32_t> * pdata = reinterpret_cast<ParamData<uint32_t>*>(data);
        if(pdata->id == _lastId)
            return;
        emit valueChanged(pdata->value);
        return;
    }

    case NODE_TYPE_FLOAT32_OPARAM :
    {
        ParamData<float> * pdata = reinterpret_cast<ParamData<float>*>(data);
        if(pdata->id == _lastId)
            return;
        emit valueChanged(pdata->value);
        return;
    }

    case NODE_TYPE_FLOAT64_OPARAM :
    {
        ParamData<double> * pdata = reinterpret_cast<ParamData<double>*>(data);
        if(pdata->id == _lastId)
            return;
        emit valueChanged(pdata->value);
        return;
    }
    }
}
