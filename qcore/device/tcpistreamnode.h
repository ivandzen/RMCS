#ifndef TCPISTREAMNODE_H
#define TCPISTREAMNODE_H
#include <core/device/istreamnode.h>
#include <QTcpServer>
#include <QMutex>
#include <QThread>
#include <QElapsedTimer>

class TcpIStreamNode;

class InputProcessingThread :
        public QThread
{
    Q_OBJECT
public :
    explicit InputProcessingThread(TcpIStreamNode * stream);

    inline Time_t time()
    {
        _mutex.lock();
        _time += _timer.restart();
        _mutex.unlock();
        return _time;
    }

    inline void start()
    {
        _time = 0;
        QThread::start();
    }

    inline void stop() { _running = false; }

protected:
    virtual void run() override;

private:
    QMutex                  _mutex;
    QElapsedTimer           _timer;
    bool                    _running;
    TcpIStreamNode *    _stream;
    Time_t                  _time;
};

///////////////////////////////////////////////////////////////

class TcpIStreamNode :
        public QObject,
        public IStreamNode
{
    Q_OBJECT
public:
    TcpIStreamNode(const char * name,
                   Length_t num_packets,
                   Device * dev,
                   const QHostAddress &address,
                   quint16 port,
                   Time_t period,
                   QObject * parent = 0);

protected:
    virtual bool lock() override;

    virtual void unlock() override;

    virtual bool nodeCustomDescriptorRequested(ControlPacket & packet) const override;

    virtual void streamToggled(bool enabled) override;

private:
    QMutex          _mutex;
    QTcpServer *    _server;
    QTcpSocket *    _connection;
    Length_t        _currentReadPos;
    InputProcessingThread *  _processingThread;
    Time_t          _period;

private slots:
    void newConnection();

    void socketReadyRead();
};

#endif // TCPISTREAMNODE_H
