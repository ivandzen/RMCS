#ifndef QOPARAMCONTROLLERWRAPPER_H
#define QOPARAMCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qoparamcontroller.h>
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

    QVariant getValue(QOParamController * node) {
        return node->getValue();
    }
};

#endif // QOPARAMCONTROLLERWRAPPER_H
