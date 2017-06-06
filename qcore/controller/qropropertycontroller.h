#ifndef QROPROPERTYCONTROLLER_H
#define QROPROPERTYCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>
#include <QVariant>

class QROPropertyController :
        public QNodeController
{
    Q_OBJECT
public:
    QROPropertyController(NodeType_t node_type,
                          NodeID_t node_id = NodeIDNull,
                          NodeID_t parent_id = NodeIDNull,
                          const QString & name = QString(),
                          QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    virtual NodeControllerForm * createForm(QWidget * parent) override;

    virtual NodeType_t type() const override;

    inline bool update() { return readData(); }

    inline QVariant get() const { return _value; }

signals:
    void updated();

protected:
    virtual bool eventSetup(const ControlPacket &) override;

    virtual bool eventData(const ControlPacket & packet) override;

    inline void setValue(const QVariant & value) { _value = value; }

private:
    NodeType_t  _type;
    QVariant    _value;
};

#endif // QROPROPERTYCONTROLLER_H
