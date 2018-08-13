#ifndef QDEVICECONTROLLER_H
#define QDEVICECONTROLLER_H

#include <QObject>
#include <core/controller/devicecontroller.h>
#include <qcore/controller/qnodecontroller.h>
#include <QHash>

class QNodeControllerFactory;

class QDeviceController :
        public QObject,
        public DeviceController
{
    Q_OBJECT
public:
    explicit QDeviceController(QDeviceConnection * connection,
                               QObject * parent = nullptr);

    virtual ~QDeviceController();

    inline QNodeController * getNode(NodeID_t node_id) const {
        return static_cast<QNodeController*>(DeviceController::getNode(node_id));
    }

    inline QNodeController * getNode(const QString & name) const {
        for(NodeID_t i = 0; i < numNodes(); ++i)
            if(getNode(i)->name() == name)
                return getNode(i);
        return nullptr;
    }

    inline QStringList nodeNames() const {
        QStringList result;
        for(NodeID_t id = 0; id < numNodes(); ++id)
            result.append(getNode(id)->name());
        return result;
    }

    inline QDeviceConnection * connection() const {
        return static_cast<QDeviceConnection*>(DeviceController::connection());
    }

    void update();

signals:
    void initialized();

    void updated();

protected:
    virtual NodeController * createNode(NodeType_t node_type,
                                        NodeID_t node_id,
                                        NodeID_t parent_id,
                                        const ArrayRef<char> & name) final override;

    virtual void logMessage(const char * message) override;

    virtual void beforeInitNodes() override;

    virtual void afterInitNodes() override;

    virtual void updatedEvent() override;

private :
    QWidget *   _form;
};

#endif // QDEVICECONTROLLER_H
