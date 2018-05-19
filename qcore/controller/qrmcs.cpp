#include "qrmcs.h"
#include <QFile>
#include <QDebug>
#include <QPluginLoader>
#include <qcore/controller/qdevicecontrollerfactory.h>
#include <qcore/controller/qnodecontrollerfactory.h>
#include <QDebug>
#include <core/common/types.h>

QRmcs * QRmcs::_instance = nullptr;

QRmcs::QRmcs(QObject * parent) :
    QThread(parent),
    _nodeFactory(new QNodeControllerFactory(this))
{
    //start();
}

QRmcs::~QRmcs()
{
    for(auto & factory : _devFactories)
        factory->deleteLater();
    exit();
}

void QRmcs::init()
{
    if(_instance != nullptr)
        return;
    qRegisterMetaType<NodeType_t>("NodeType_t");
    qRegisterMetaType<NodeID_t>("NodeID_t");
    qRegisterMetaType<CtlReq_t>("CtlReq_t");
    qRegisterMetaType<Data>("Data");
    qRegisterMetaType<DataPtr>("DataPtr");
    qRegisterMetaType<Length_t>("Length_t");
    qRegisterMetaType<NodeStatus_t>("NodeStatus_t");
    qRegisterMetaType<Time_t>("Time_t");
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<QList<NodeID_t>>("QList<NodeID_t>");

    _instance = new QRmcs(nullptr);
    _instance->start();
}

void QRmcs::finalize()
{
    if(_instance == nullptr)
        return;
    _instance->exit(0);
    _instance->deleteLater();
}

QDeviceControllerFactory * QRmcs::loadFactory(const QString & filename)
{
    QFile file(filename);
    if(!file.exists())
    {
        qDebug() << QString("Plug-in file %1 not exists").arg(filename);
        return nullptr;
    }

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << QString("Unable to open plug-in file %1").arg(filename);
        return nullptr;
    }
    file.close();

    QPluginLoader loader(filename);
    if(!loader.load())
    {
        qDebug() << "Plug-in not loaded";
        return nullptr;
    }

    QDeviceControllerFactory * factory = qobject_cast<QDeviceControllerFactory*>(loader.instance());

    if(factory == nullptr)
    {
        qDebug() << "Unable to instantiate plug-in";
        return nullptr;
    }

    if(!addFactory(factory))
    {
        factory->deleteLater();
        return nullptr;
    }

    factory->setContext(this);
    return factory;
}

bool QRmcs::addFactory(QDeviceControllerFactory *factory)
{
    QString factory_name = factory->factoryName();

    if(_devFactories.contains(factory_name))
    {
        qDebug() << QString("Device factory named '%1' already loaded").arg(factory_name);
        return false;
    }

    factory->moveToThread(this);
    _devFactories.insert(factory_name, factory);
    return true;
}

QStringList QRmcs::factoriesNames() const
{
    return _devFactories.keys();
}

QDeviceControllerFactory * QRmcs::getFactory(const QString &factory_name) const
{
    if(!_devFactories.contains(factory_name))
        return nullptr;

    return _devFactories[factory_name];
}

void QRmcs::run()
{
//    while(true)
//    {
//        qDebug() << "Hi!!!";
//        msleep(1000);
//    }

    qDebug() << "Rmcs started";
    exec();
}
