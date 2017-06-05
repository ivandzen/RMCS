#ifndef QRWPROPERTYCONTROLLER_H
#define QRWPROPERTYCONTROLLER_H
#include <qcore/controller/properties/qropropertycontroller.h>
#include <QVariant>

class QRWPropertyController :
        public QROPropertyController
{
    Q_OBJECT
public:
    QRWPropertyController(NodeType_t node_type,
                          NodeID_t node_id = NodeIDNull,
                          NodeID_t parent_id = NodeIDNull,
                          const QString & name = QString(),
                          QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    virtual NodeControllerForm * createForm(QWidget * parent) override;

    bool set(const QVariant & value);

protected:
    virtual bool eventData(const ControlPacket & packet) override;
};

#endif // QRWPROPERTYCONTROLLER_H
