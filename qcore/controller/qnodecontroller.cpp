#include "qnodecontroller.h"
#include "qdevicecontroller.h"
#include <QTime>
#include <QDebug>

QNodeController::QNodeController(NodeID_t node_id,
                                 NodeID_t parent_id,
                                 const QString & name,
                                 QDeviceConnection * dev) :
    QObject(nullptr),
    NodeController(node_id, parent_id, name.toStdString(), dev)
{
    setObjectName(name);
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::update()
{
    readData();
    QObjectList childs = children();
    for(QObject * obj : childs)
        if(QNodeController * child = qobject_cast<QNodeController*>(obj))
            child->update();
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::eventDestroy()
{
    QObject::deleteLater();
}

////////////////////////////////////////////////////////////////////////////////

bool QNodeController::eventInit(DeviceController * controller)
{
    (void)controller;
    emit initialized();
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::eventStatus(NodeStatus_t status)
{
    (void)status;
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::logMessage(const char * message)
{
    QTime now = QTime::currentTime();
    QString formatted = QString("%1 >>> %2{ Name = %3, ID = %4 } says : %5")
                                .arg(now.toString("hh:mm:ss"))
                                .arg(metaObject()->className())
                                .arg(name())
                                .arg(id())
                                .arg(message);
    qDebug() << formatted; //! @attention еще раз подумать
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::logMessage(const QString & message)
{
    logMessage(message.toUtf8().data());
}
