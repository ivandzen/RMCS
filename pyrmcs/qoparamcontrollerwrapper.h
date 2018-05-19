#ifndef QOPARAMCONTROLLERWRAPPER_H
#define QOPARAMCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qoparamcontroller.h>
#include <qcore/controller/qostreamchannelcontroller.h>
#include <core/common/defaultnodes.h>

class QOParamControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public slots:
    QOParamController * new_QOParamController(QNodeController * node) {
        if(!isOParamType(node->type()))
            return nullptr;
        return static_cast<QOParamController*>(node);
    }

    QOParamController * new_QOParamController(QOStreamChannelController * channel) {
        if(!isOParamType(channel->type()))
            return nullptr;
        return static_cast<QOParamController*>(channel);
    }

    //QVariant popValue(QOParamController * node) {
    //    return node->popValue();
    //}

    //void reset(QOParamController * param) {
    //    param->reset();
    //}

    //void setBufferSize(QOParamController * param, int size) {
    //    param->setBufferSize(size);
    //}
};

#endif // QOPARAMCONTROLLERWRAPPER_H
