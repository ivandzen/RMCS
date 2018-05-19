#include "qoparamcontroller.h"
#include <core/common/defaultnodes.h>
#include <QDebug>

QOParamController::QOParamController(NodeType_t type,
                                     NodeID_t node_id,
                                     NodeID_t parent_id,
                                     const QString & name,
                                     QDeviceConnection * dev) :
    QOStreamChannelController (node_id, parent_id, name, dev),
    _type(type)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////

QInt16OParamController::QInt16OParamController(NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const QString &name,
                                               QDeviceConnection *dev) :
    QOParamController(NODE_TYPE_INT16_OPARAM,
                      node_id, parent_id, name, dev),
    _lastId(0),
    _consumer(nullptr)
{}

void QInt16OParamController::streamDataReceived(Data * data)
{
    _ParamData<int16_t> pdata = _ParamData<int16_t>(data, count());

    if(!pdata.isValid())
        return;

    ParamDataID did = pdata.getID();
    if((did == 0) || (did == _lastId))
        return;

    _lastId = did;
    if(_consumer)
        _consumer->execute(pdata);
}
