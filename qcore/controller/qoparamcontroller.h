#ifndef QOPARAMCONTROLLER_H
#define QOPARAMCONTROLLER_H
#include <qcore/controller/qostreamchannelcontroller.h>
#include <QVariant>

class QOParamController :
        public QOStreamChannelController
{
    Q_OBJECT
public :
    QOParamController(NodeType_t type,
                      NodeID_t node_id = NodeIDNull,
                      NodeID_t parent_id = NodeIDNull,
                      const QString & name = QString(),
                      QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    virtual NodeType_t type() const override { return _type; }

    inline QVariant getValue() const { return _value; }

signals :
    void valueChanged(QVariant value);

protected:
    virtual void streamDataReceived(Data * data) override;

private:
    NodeType_t  _type;
    uint8_t     _lastId;
    QVariant    _value;
};

#endif // QOPARAMCONTROLLER_H
