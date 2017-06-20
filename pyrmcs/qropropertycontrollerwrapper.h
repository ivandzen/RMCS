#ifndef QROPROPERTYCONTROLLERWRAPPER_H
#define QROPROPERTYCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qropropertycontroller.h>
#include <core/common/defaultnodes.h>

class QROPropertyControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public slots:
    QROPropertyController * new_QROPropertyController(QNodeController * node) {
        if(!isROPropertyType(node->type()))
            return nullptr;
        return static_cast<QROPropertyController*>(node);
    }

    void update(QROPropertyController * prop) { prop->update(); }

    QVariant get(QROPropertyController * prop) const { return prop->get(); }
};

#endif // QROPROPERTYCONTROLLERWRAPPER_H
