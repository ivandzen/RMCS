#include "tcpbackend.h"
#include <common/control_protocol.h>
#include <QElapsedTimer>
#include <QThread>

class TcpStreamReaderThread :
        public QThread
{
    Q_OBJECT
};

class TcpStreamWriterThread :
        public QThread
{
    Q_OBJECT
};

TcpBackend::TcpBackend(QObject * parent) :
    QObject(parent),
    _controlSocket(new QTcpSocket(this))
{
    connect(_controlSocket, SIGNAL(readyRead(void)),
            this, SLOT(controlReadyRead(void)));
    connect(_controlSocket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(bytesWritten(qint64)));
}

bool TcpBackend::init(qint64 timeout)
{
    /// Основное назначение инициализации бэкенда -
    /// определить порты, к которым привязаны входящий и
    /// выходящий потоки данных на устройстве
    controlHeader()->type = TT_INIT;
    controlHeader()->ep_id = NodeIDNull;
    controlHeader()->req = CTLREQ_NONE;
    controlHeader()->length = 0;

    _controlSocket->write((const char*)controlData(), sizeof(ControlHeader));
    QElapsedTimer timer;
    timer.start();
    if(_controlSocket->waitForBytesWritten(timeout))
    {
        qint64 elapsed = timer.restart();

    }
}

ControlPacket TcpBackend::beginControlTransaction(Length_t max_length)
{
    _mutex.lock();
    if(_controlPacket.size() < (sizeof(ControlHeader) + max_length))
    {
        _controlPacket.resize(sizeof(ControlHeader) + max_length);
    }
    return ControlPacket(ArrayRef<Data>(controlData() + sizeof(ControlHeader),
                                        Length_t(_controlPacket.size())));
}

bool TcpBackend::endControlTransaction(NodeID_t ep_id,
                                       CtlReq_t req,
                                       const ControlPacket & packet,
                                       Time_t timeout,
                                       const DeviceConnection::TransmitHandler & handler)
{
    controlHeader()->type = TT_CTL_WRITE;
    controlHeader()->ep_id = ep_id;
    controlHeader()->req = req;
    controlHeader()->length = packet.payloadLength();
    _controlSocket->write((const char*)controlData(), controlHeader()->totalLength());
    if(_controlSocket->waitForBytesWritten(timeout))
    {
        handler();
        _mutex.unlock();
        return true;
    }

    _mutex.unlock();
    return false;
}

bool TcpBackend::requestControlPacket(NodeID_t ep_id,
                                      CtlReq_t req,
                                      Length_t max_length,
                                      Time_t timeout,
                                      const DeviceConnection::ReceiveHandler &handler)
{
    controlHeader()->type = TT_CTL_REQ;
    controlHeader()->ep_id = ep_id;
    controlHeader()->req = req;
    controlHeader()->length = max_length;
    _controlSocket->write((const char*)controlData(), sizeof(ControlHeader));
    _timerId = startTimer(timeout);


    QElapsedTimer timer;
    timer.start();

    if(_controlSocket->waitForBytesWritten(timeout))
    {
        qint64 elapsed = timer.restart();
        _controlSocket->read((char*)controlData(), sizeof(ControlPacket) + max_length);
        if(_controlSocket->waitForReadyRead(timeout - elapsed))
        {
            handler(ControlPacket(ArrayRef<Data>(controlData() + sizeof(ControlHeader),
                                                 controlHeader()->length)));
            return true;
        }
    }
    return false;
}

void TcpBackend::timerEvent(QTimerEvent *event)
{

}

void TcpBackend::controlReadyRead()
{

}

void TcpBackend::bytesWritten(qint64)
{

}
