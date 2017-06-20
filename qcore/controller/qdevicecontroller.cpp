#include "qdevicecontroller.h"
#include <QDebug>
#include <qcore/controller/qnodecontrollerfactory.h>
#include <qcore/controller/qrmcs.h>

QDeviceController::QDeviceController(QDeviceConnection * connection,
                                     QObject * parent) :
    QObject(parent),
    DeviceController(connection),
    _form(nullptr)
{
}

QDeviceController::~QDeviceController()
{
    //! @todo
}

void QDeviceController::update()
{
    QObjectList childs = children();
    for(QObject * obj : childs)
        if(QNodeController * child = qobject_cast<QNodeController*>(obj))
            child->update();
}

NodeController * QDeviceController::createNode(NodeType_t node_type,
                                               NodeID_t node_id,
                                               NodeID_t parent_id,
                                               const ArrayRef<char> & name)
{
    QString qname(QByteArray(name.data(), name.size()));

    QNodeController * new_node = QRmcs::instance()->nodeFactory()->createNode(node_type,
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

void QDeviceController::beforeInitNodes()
{
    for(NodeID_t i = 0; i < numNodes(); ++i)
    {
        QNodeController * node = getNode(i);
        if(node)
        {
            if(node->parentId() != NodeIDNull)
            {
                QNodeController * parent = getNode(node->parentId());
                if(parent)
                    node->setParent(parent);
                else
                    logMessage("Parent node not found");
            }
            else
                node->setParent(this);
        }
    }
}

void QDeviceController::afterInitNodes()
{
    emit initialized();
}
