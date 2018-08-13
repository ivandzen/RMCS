#ifndef QOPARAMCONTROLLER_H
#define QOPARAMCONTROLLER_H
#include <qcore/controller/qostreamchannelcontroller.h>
#include <functional>
#include <QVariant>
#include <QQueue>
#include <QMutex>
#include <QVector>
#include <QDebug>

////////////////////////////////////////////////////////////////////////////////////////////////

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

    virtual NodeType_t type() const override { return _type; }

private:
    NodeType_t      _type;
};

////////////////////////////////////////////////////////////////////////////////////////////////

class QInt16OParamController :
        public QOParamController
{
    Q_OBJECT
public:
    class Consumer
    {
    public :
        virtual void execute(const _ParamData<int16_t> & data) = 0;

        virtual ~Consumer() {}
    };

    QInt16OParamController(NodeID_t node_id = NodeIDNull,
                           NodeID_t parent_id = NodeIDNull,
                           const QString & name = QString(),
                           QDeviceConnection * dev = nullptr);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override
    {
        return new QInt16OParamController(node_id, parent_id, name, device);
    }

    inline void setConsumer(Consumer * consumer) {
        _consumer = consumer;
    }

protected:
    virtual void streamDataReceived(Data * data) final override;

protected:
    ParamDataID _lastId;
    Consumer *  _consumer;
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif // QOPARAMCONTROLLER_H
