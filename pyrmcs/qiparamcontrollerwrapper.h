#ifndef QIPARAMCONTROLLERWRAPPER_H
#define QIPARAMCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qiparamcontroller.h>
#include <core/common/defaultnodes.h>

class QIParamControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public slots:
    QIParamController * new_QIParamController(QNodeController * node) {
        if(!isIParamType(node->type()))
            return nullptr;
        return static_cast<QIParamController*>(node);
    }

    void setValue(QIParamController * node,
                  const QVariant & value) {
        node->setValue(value);
    }
};

#endif // QIPARAMCONTROLLERWRAPPER_H
