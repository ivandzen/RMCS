#ifndef QNODECONTROLLERWRAPPER_H
#define QNODECONTROLLERWRAPPER_H
#include <qcore/controller/qnodecontroller.h>

class QNodeControllerWrapper :
        public QObject
{
    Q_OBJECT
public slots:
    NodeID_t id(QNodeController * controller) const {
        return controller->id();
    }

    NodeID_t parentId(QNodeController * controller) const {
        return controller->parentId();
    }

    QStringList childNames(QNodeController * controller) {
        QStringList result;
        QObjectList children = controller->children();
        for(QObject * obj : children)
            if(QNodeController * child = qobject_cast<QNodeController*>(obj))
                result.push_back(child->name());
        return result;
    }

    QString name(QNodeController * controller) {
        return controller->name();
    }

    NodeType_t type(QNodeController * controller) {
        return controller->type();
    }
};

#endif // QNODECONTROLLERWRAPPER_H
