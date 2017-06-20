#ifndef QNODECONTROLLER_H
#define QNODECONTROLLER_H

#include <QObject>
#include <core/controller/nodecontroller.h>
#include <qcore/controller/qdeviceconnection.h>

class QNodeController :
        public QObject,
        public NodeController
{
    Q_OBJECT
public:
    explicit QNodeController(NodeID_t node_id,
                             NodeID_t parent_id,
                             const QString & name,
                             QDeviceConnection * dev);

    inline QString name() const { return QString(NodeController::name().data()); }

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) = 0;

    virtual NodeType_t type() const = 0;

    void update();

protected:
    virtual void eventDestroy() override;

    virtual bool eventInit(DeviceController * controller) override;

    virtual void eventStatus(NodeStatus_t status) override;

    virtual void logMessage(const char * message) override;

signals:
    void initialized();
};

#endif // QNODECONTROLLER_H
