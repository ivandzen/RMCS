#include "qtcpistreamcontroller.h"
#include "common/tcp.h"

QTcpIStreamController::QTcpIStreamController(NodeID_t id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceController *device,
                                             QObject * parent) :
    QIStreamController(id, parent_id, name, device, parent),
    _socket(new QTcpSocket(this))
{
    connect(_socket, SIGNAL(disconnected()),
            this, SLOT(socketDisconnected()));
}

QNodeController * QTcpIStreamController::createInstance(NodeID_t node_id,
                                                        NodeID_t parent_id,
                                                        const QString & name,
                                                        QDeviceController * device)
{
    return new QTcpIStreamController(node_id,
                                     parent_id,
                                     name,
                                     device);
}

NodeControllerForm *QTcpIStreamController::createForm(QWidget * parent)
{
    //! @todo
    return nullptr;
}

bool QTcpIStreamController::parseCustomDescriptor(const ControlPacket & packet)
{
    TcpIStreamDescriptor descriptor(packet);
    if(!descriptor.isValid())
        return false;
    if(descriptor.packetSize() == 0)
    {
        logMessage("Zero stream packet size?!");
        return true;
    }

    initWriter(descriptor.packetSize(),
                 descriptor.period());
    _hostAddress = descriptor.address();
    _port = descriptor.port();
    return true;
}

void QTcpIStreamController::streamToggled(bool enabled)
{
    QIStreamController::streamToggled(enabled);
    if(enabled)
        _socket->connectToHost(_hostAddress,
                               _port,
                               QIODevice::WriteOnly);
    else
        _socket->close();
}

bool QTcpIStreamController::init()
{
    bool ok = false;
    _hostAddress.toIPv4Address(&ok);
    return ok && (_port > QISTREAM_MIN_PORT_NUMBER);
}

void QTcpIStreamController::sendStreamData(const QByteArray & data)
{
    if(_socket->isOpen())
        _socket->write(data);
}

void QTcpIStreamController::socketDisconnected()
{
    logMessage("Socket disconnected. Stopping writer thread.");
    connectionBreak();
}
