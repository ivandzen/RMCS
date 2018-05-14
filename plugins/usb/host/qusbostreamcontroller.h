#ifndef QUSBBULKOSTREAMCONTROLLER_H
#define QUSBBULKOSTREAMCONTROLLER_H
#include <qcore/controller/qostreamcontroller.h>
#include <usb/common/common.h>
#include <libusb.h>
#include <QThread>
#include <QSemaphore>
#include <QMutexLocker>
#include <QElapsedTimer>

class QUsbOStreamController;

class QUsbOStreamProcessor :
        public QThread
{
    Q_OBJECT
public:
    explicit QUsbOStreamProcessor(QUsbOStreamController * controller);

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
    QUsbOStreamController * _controller;
    Length_t                _currentPacket;
};

///////////////////////////////////////////////////////////////////////

class QUSBOStreamReader :
        public QThread
{
    Q_OBJECT
    friend void CALLBACK_ATTRIB ostreamTransferCallback(libusb_transfer * transfer);

public:
    QUSBOStreamReader(UsbEPDescriptor::EPType_t type,
                      QUsbOStreamController * controller);

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

signals:
    void shutDown();

protected:
    virtual void run() final override;

private:
    void beginTransfer();

    void transferCplt(struct libusb_transfer * tfer);

    void logMessage(const QString & message);

    bool                    _running;
    QMutex                  _mutex;
    QUsbOStreamController * _controller;
    int                     _currentPacket;
    struct libusb_transfer * _transfer;
    uint16_t                _period;
    UsbEPDescriptor::EPType_t _type;
};

///////////////////////////////////////////////////////////////////////

class QUsbOStreamController :
        public QOStreamController
{
    Q_OBJECT
    friend class QUsbOStreamProcessor;
    friend class QUSBOStreamReader;
    friend class QUSBOStreamReader;

public:
    static const Length_t NUM_PACKETS = 32;
    static const Length_t PACKETS_PER_TRANSFER = 1;

    QUsbOStreamController(libusb_device_handle * usb_handle,
                          QDeviceConnection * conn = nullptr,
                          NodeID_t node_id = NodeIDNull,
                          NodeID_t parent_id = NodeIDNull,
                          const QString & name = QString());

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * conn) override;

    virtual NodeType_t type() const final override { return NODE_TYPE_USBOSTREAM; }

protected:
    virtual void eventDestroy() override;

    virtual bool eventInit(DeviceController * controller) override;

    virtual bool eventSetup(const ControlPacket & packet) override;

    virtual void eventStreamToggled(const QVariant & enabled) override;

private:
    inline unsigned int timeout() const { return PACKETS_PER_TRANSFER * 1000; }

    QUsbOStreamProcessor *  _processor;
    QUSBOStreamReader * _reader;
    libusb_device_handle *  _handle;
    uint8_t                 _epAddr;
    Length_t                _packetSize;
    QByteArray              _buffer;
    QSemaphore              _readyPackets;
    QSemaphore              _freePackets;

private slots:
    void onReaderShutDown();
};

#endif // QUSBBULKOSTREAMCONTROLLER_H
