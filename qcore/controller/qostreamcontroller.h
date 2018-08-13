#ifndef QOSTREAMCONTROLLER_H
#define QOSTREAMCONTROLLER_H
#include <QVector>
#include <qcore/controller/qrwpropertycontroller.h>
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qostreamchannelcontroller.h>
#include <QThread>
#include <QMutexLocker>
#include <QSemaphore>

class QOStreamController;

class QOStreamProcessor :
        public QThread
{
    Q_OBJECT
public:
    explicit QOStreamProcessor(QOStreamController * controller);

    inline void start()
    {
        QMutexLocker locker(&_mutex);
        if(_running) return;
        _running = true;
        QThread::start();
    }

    inline void stop()
    {
        QMutexLocker locker(&_mutex);
        _running = false;
    }

protected:
    virtual void run() final override;

private:
    bool                    _running;
    QMutex                  _mutex;
    QOStreamController *    _controller;
    Length_t                _currentPacket;
};

////////////////////////////////////////////////////////////////////////////////

class QOStreamReader :
        public QThread
{
    Q_OBJECT
public:
    typedef enum
    {
        TFER_COMPLETE = 0,
        TFER_ERROR = 1,
        TFER_STOP = 2
    }
    TferResult;

    QOStreamReader(uint16_t period,
                   QOStreamController * controller);

    void endTransfer(Length_t readyPackets,
                     TferResult res);

    inline void start()
    {
        QMutexLocker locker(&_mutex);
        if(_running) return;
        _running = true;
        QThread::start();
    }

    inline void stop()
    {
        QMutexLocker locker(&_mutex);
        _running = false;
    }

    inline Length_t packetSize() const;

signals:
    void shutDown();

protected:

    virtual void run() final override;

    virtual bool beginTransfer(char * buffer,
                               Length_t numPackets,
                               Length_t packSize) = 0;

    void logMessage(const QString & message);

private:
    bool                    _running;
    QMutex                  _mutex;
    QOStreamController *    _controller;
    int                     _currentPacket;
    uint16_t                _period;
};

////////////////////////////////////////////////////////////////////////////////

class QOStreamController :
        public QNodeController
{
    Q_OBJECT
    friend class QOStreamProcessor;
    friend class QOStreamReader;
    friend class QOStreamChannelController;

public:
    QOStreamController(Length_t buf_cap,
                       Length_t ppt,
                       NodeID_t node_id,
                       NodeID_t parent_id,
                       const QString & name,
                       QDeviceConnection *conn);

    ~QOStreamController();

    inline QRWPropertyController * enabled() { return _enabled; }

    bool setBufCapacity(Length_t cap);

    bool setPPT(Length_t ppt);

    inline uint16_t numChannels() const {
        return uint16_t(_channels.size());
    }

    inline QOStreamChannelController * getChannel(uint16_t number) const {
        return _channels[number];
    }

protected:
    virtual bool eventInit(DeviceController * device) final override;

    virtual bool eventData(const ControlPacket & packet) final override;

    bool setPacketSize(Length_t pack_size);

    bool setReader(QOStreamReader * reader);

protected slots:
    void eventStreamToggled(const QVariant & enabled);

    void onReaderShutDown();

private:
    bool addChannel(QOStreamChannelController * channel);

    void processChannels(const ArrayRef<Data> & buffer);

    typedef QVector<QOStreamChannelController *> ChannelList;
    QRWPropertyController * _enabled = nullptr;
    ChannelList             _channels;
    Length_t                _packetSize;
    Length_t                _bufCapacity;
    Length_t                _ppt;
    QByteArray              _buffer;
    QSemaphore              _readyPackets;
    QSemaphore              _freePackets;
    QOStreamProcessor *     _processor;
    QOStreamReader *        _reader;
};

inline Length_t QOStreamReader::packetSize() const { return _controller->_packetSize; }

#endif // QOSTREAMCONTROLLER_H
