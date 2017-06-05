#ifndef QDEVICECONNECTION_H
#define QDEVICECONNECTION_H
#include <QObject>
#include <core/controller/deviceconnection.h>

class QNodeController;

class QDeviceConnection :
        public QObject,
        public DeviceConnection
{
    Q_OBJECT
public:
    explicit QDeviceConnection(QObject * parent = nullptr);

    virtual QNodeController * createNode(NodeType_t node_type,
                                         NodeID_t node_id,
                                         NodeID_t parent_id,
                                         const QString & name) = 0;
};

#endif // QDEVICECONNECTION_H
