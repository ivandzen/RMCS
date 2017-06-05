#include "qdevicecontroller.h"
#include <QDebug>
#include <qcore/controller/qnodecontrollerfactory.h>

QDeviceController::QDeviceController(QDeviceConnection * connection,
                                     QNodeControllerFactory * factory,
                                     QObject * parent) :
    QObject(parent),
    DeviceController(connection),
    _factory(factory)
{
}

QDeviceController::~QDeviceController()
{
    //! @todo
}

NodeController * QDeviceController::createNode(NodeType_t node_type,
                                               NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const ArrayRef<char> & name)
{
    QString qname(QByteArray(name.data(), name.size()));

    QNodeController * new_node = _factory->createNode(node_type,
                                                      node_id,
                                                      parent_id,
                                                      qname,
                                                      connection());

    if(new_node == nullptr)
        new_node = connection()->createNode(node_type,
                                            node_id,
                                            parent_id,
                                            qname);

    if(new_node != nullptr)
        new_node->moveToThread(thread());

    return new_node;
}

void QDeviceController::logMessage(const char *message)
{
    qDebug() << message;
}
