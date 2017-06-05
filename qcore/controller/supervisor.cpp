#include "supervisor.h"
#include <QFile>
#include <QDebug>
#include <QPluginLoader>
#include <qcore/controller/qdevicecontrollerfactory.h>
#include <QDebug>

QRmcs::QRmcs(QObject * parent) :
    QThread(parent)
{
    //start();
}

QRmcs::~QRmcs()
{
    for(auto & factory : _factories)
        factory->deleteLater();
    exit();
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

    factory->moveToThread(this);
    return factory;
}

bool QRmcs::addFactory(QDeviceControllerFactory *factory)
{
    QString factory_name = factory->factoryName();

    if(_factories.contains(factory_name))
    {
        qDebug() << QString("Device factory named '%1' already loaded").arg(factory_name);
        return false;
    }

    factory->moveToThread(this);
    _factories.insert(factory_name, factory);
    return true;
}

QStringList QRmcs::factoriesNames() const
{
    return _factories.keys();
}

QDeviceControllerFactory * QRmcs::getFactory(const QString &factory_name) const
{
    if(!_factories.contains(factory_name))
        return nullptr;

    return _factories[factory_name];
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
