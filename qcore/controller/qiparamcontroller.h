#ifndef QIPARAMCONTROLLER_H
#define QIPARAMCONTROLLER_H
#include <qcore/controller/qistreamchannelcontroller.h>
#include <QVariant>

class QIParamController :
        public QIStreamChannelController
{
public:
    QIParamController(NodeType_t type,
                      NodeID_t node_id = NodeIDNull,
                      NodeID_t parent_id = NodeIDNull,
                      const QString & name = QString(),
                      QDeviceConnection * connection = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    virtual NodeControllerForm * createForm(QWidget * parent) override;

    virtual NodeType_t type() const override { return _type; }

public slots:
    void setValue(const QVariant & value);

protected:
    virtual void fill(ArrayRef<Data> & arr) const override;

private:
    NodeType_t  _type;
    QVariant    _value;
    uint8_t     _dataId;
    bool        _ready;
};

#endif // QIPARAMCONTROLLER_H
