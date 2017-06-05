#include "qnodecontrollerfactory.h"
#include <QFile>
#include <QDebug>
#include <QPluginLoader>

#include <qcore/controller/qdefaultnode.h>
#include <qcore/controller/channels/channels.h>
#include <qcore/controller/properties/qropropertycontroller.h>
#include <qcore/controller/properties/qrwpropertycontroller.h>

QNodeControllerFactory::QNodeControllerFactory(QObject *parent) :
    QObject(parent)
{
    addPrototype(new QDefaultNode());

    addPrototype(new QUInt8IStreamChannel());
    addPrototype(new QUInt16IStreamChannel());
    addPrototype(new QUInt32IStreamChannel());
    addPrototype(new QFloat32IStreamChannel());
    addPrototype(new QFloat64IStreamChannel());

    addPrototype(new QUInt8OStreamChannel());
    addPrototype(new QUInt16OStreamChannel());
    addPrototype(new QUInt32OStreamChannel());
    addPrototype(new QFloat32OStreamChannel());
    addPrototype(new QFloat64OStreamChannel());

    addPrototype(new QROPropertyController(NODE_TYPE_BOOL_PROP_RDONLY));
    addPrototype(new QROPropertyController(NODE_TYPE_INT_PROP_RDONLY));
    addPrototype(new QROPropertyController(NODE_TYPE_FLOAT_PROP_RDONLY));

    addPrototype(new QRWPropertyController(NODE_TYPE_BOOL_PROP));
    addPrototype(new QRWPropertyController(NODE_TYPE_INT_PROP));
    addPrototype(new QRWPropertyController(NODE_TYPE_FLOAT_PROP));
}

bool QNodeControllerFactory::loadPrototype(const QString & filename)
{
    QFile file(filename);
    if(!file.exists())
    {
        qDebug() << QString("Plug-in file %1 not exists").arg(filename);
        return false;
    }

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << QString("Unable to open plug-in file %1").arg(filename);
        return false;
    }
    file.close();

    QPluginLoader loader(filename);
    if(!loader.load())
    {
        qDebug() << "Plug-in not loaded";
        return false;
    }

    QNodeController * node = qobject_cast<QNodeController*>(loader.instance());

    if(node == nullptr)
    {
        qDebug() << "Unable to instantiate plug-in";
        return false;
    }

    if(!addPrototype(node))
    {
        node->deleteLater();
        return false;
    }

    return true;
}

bool QNodeControllerFactory::addPrototype(QNodeController * node)
{
    if(_prototypes.contains(node->type()))
    {
        qDebug() << "Node already registered";
        return false;
    }
    _prototypes.insert(node->type(), node);
    return true;
}

QNodeController *QNodeControllerFactory::createNode(NodeType_t node_type,
                                                    NodeID_t node_id,
                                                    NodeID_t parent_id,
                                                    const QString & name,
                                                    QDeviceConnection * device)
{
    if(!_prototypes.contains(node_type))
        return nullptr;
    return _prototypes[node_type]->createInstance(node_id,
                                                  parent_id,
                                                  name,
                                                  device);
}
