#ifndef QUSBOSTREAMCONTROLLER_H
#define QUSBOSTREAMCONTROLLER_H
#include <qcore/controller/qostreamcontroller.h>
#include <usb/common/common.h>
#include <libusb.h>
#include <QThread>
#include <QSemaphore>
#include <QMutexLocker>

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

    inline void stop() {
        QMutexLocker locker(&_mutex);
        _running = false;
    }

protected:
    virtual void run() override;

private:
    bool        _running;
    QMutex      _mutex;
    QUsbOStreamController * _controller;
    Length_t    _currentPacket;
};

///////////////////////////////////////////////////////////////////////

class QUsbOStreamController :
        public QOStreamController
{
    Q_OBJECT
    friend class QUsbOStreamProcessor;
    friend void ostreamTransferCallback(libusb_transfer * transfer);
public:
    static const Length_t NUM_PACKETS = 64;
    static const Length_t PACKETS_PER_TRANSFER = 8;

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

    virtual void eventStreamToggled(bool enabled) override;

private:
    inline unsigned int timeout() const { return PACKETS_PER_TRANSFER; }

    void beginTransfer();

    void transferCplt(libusb_transfer * tfer);

    QUsbOStreamProcessor *  _processor;
    libusb_device_handle *  _handle;
    uint8_t                 _epAddr;
    Length_t                _packetSize;
    QByteArray              _buffer;
    QSemaphore              _freePackets;
    QSemaphore              _usedPackets;
    Length_t                _currentPacket;
};

#endif // QUSBOSTREAMCONTROLLER_H
