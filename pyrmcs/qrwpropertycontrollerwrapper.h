#ifndef QRWPROPERTYCONTROLLERWRAPPER_H
#define QRWPROPERTYCONTROLLERWRAPPER_H
#include <qnodecontrollerwrapper.h>
#include <qcore/controller/qrwpropertycontroller.h>

class QRWPropertyControllerWrapper :
        public QNodeControllerWrapper
{
    Q_OBJECT
public slots:
    QRWPropertyController * new_QRWPropertyController(QNodeController * node) {
        return static_cast<QRWPropertyController*>(node);
    }

    void update(QRWPropertyController * prop) { prop->update(); }

    QVariant get(QRWPropertyController * prop) const { return prop->get(); }

    bool set(QRWPropertyController * prop,
             const QVariant & value) const { return prop->set(value); }
};
#endif // QRWPROPERTYCONTROLLERWRAPPER_H
