#include "tcpistreamnode.h"
#include "common/tcp.h"
#include <QTcpSocket>
#include <QElapsedTimer>

template<typename T>
inline T min(T a, T b) { return a < b ? a : b; }

TcpIStreamNode::TcpIStreamNode(const char * name,
                               Length_t num_packets,
                               Device * dev,
                               const QHostAddress & address,
                               quint16 port,
                               Time_t period,
                               QObject * parent) :
    QObject(parent),
    IStreamNode(name, NODE_TYPE_TCP_INPUT_STREAM, num_packets, dev),
    _server(new QTcpServer(this)),
    _connection(nullptr),
    _processingThread(new InputProcessingThread(this)),
    _period(period)
{
    _server->setMaxPendingConnections(1);
    connect(_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    _server->listen(address, port);
}

bool TcpIStreamNode::lock()
{
    return _mutex.tryLock();
}

void TcpIStreamNode::unlock()
{
    _mutex.unlock();
}

bool TcpIStreamNode::nodeCustomDescriptorRequested(ControlPacket & packet) const
{
    TcpIStreamDescriptor descriptor(packet);
    return descriptor.init(_period,
                           packetSize(),
                           _server->serverPort(),
                           _server->serverAddress());
}

void TcpIStreamNode::streamToggled(bool enabled)
{
    if(!_connection)
        return;

    if(enabled)
    {
        _currentReadPos = 0;
        _processingThread->start();
        connect(_connection, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
        beginSync(_processingThread->time());
    }
    else
    {
        disconnect(this, SLOT(socketReadyRead()));
        _processingThread->stop();
    }
}

void TcpIStreamNode::newConnection()
{
    _connection = _server->nextPendingConnection();
}

void TcpIStreamNode::socketReadyRead()
{
    qint64 available = _connection->bytesAvailable();
    qint64 ready = 0;
    while(ready < available)
    {
        qint64 to_read = min(available - ready, qint64(streamBufferSize() - _currentReadPos));
        qint64 tmp_ready = _connection->read(reinterpret_cast<char*>(streamBuffer() + _currentReadPos), to_read);
        if(tmp_ready > 0)
        {
            _currentReadPos += tmp_ready;
            ready += tmp_ready;
            if(_currentReadPos == streamBufferSize())
            {
                endSync(_processingThread->time());
                _currentReadPos = 0;
                beginSync(_processingThread->time());
            }
        }
        else
        {
            //! @todo обработать ошибку
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////

InputProcessingThread::InputProcessingThread(TcpIStreamNode *stream) :
    QThread(stream),
    _stream(stream),
    _time(0)
{}

void InputProcessingThread::run()
{
    _timer.start();
    _running = true;
    while(_running)
    {
        (void)time();
        _stream->process(_time);
    }
}
