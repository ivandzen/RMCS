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

}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::eventDestroy()
{
    QObject::deleteLater();
}

////////////////////////////////////////////////////////////////////////////////

bool QNodeController::eventInit(DeviceController * controller)
{
    emit initialized();
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void QNodeController::eventStatus(NodeStatus_t status)
{
    //! @todo
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
