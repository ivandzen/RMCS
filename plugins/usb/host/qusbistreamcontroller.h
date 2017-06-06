#ifndef QUSBISTREAMNODE_H
#define QUSBISTREAMNODE_H
#include <qcore/controller/qistreamcontroller.h>
#include <usb/common/common.h>
#include <libusb.h>
#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>

class QUsbIStreamController;

class QUsbIStreamProcessor :
        public QThread
{
    Q_OBJECT

public:
    explicit QUsbIStreamProcessor(QUsbIStreamController * controller);

    inline void start()
    {
        QMutexLocker locker(&_mutex);
        if(_running)
            return;
        QThread::start();
    }

    inline void stop() {
        QMutexLocker locker(&_mutex);
        _running = false;
    }

protected:
    virtual void run() override;

private:
    QUsbIStreamController * _controller;
    bool        _running;
    QMutex      _mutex;
};

///////////////////////////////////////////////////////////////////////

class QUsbIStreamController :
        public QIStreamController
{
    Q_OBJECT
    friend class QUsbIStreamProcessor;
    friend void __attribute__((__stdcall__)) istreamTransferCallback(libusb_transfer * transfer);
public:
    QUsbIStreamController(libusb_device_handle * usb_handle,
                          QDeviceConnection *dev = nullptr,
                          NodeID_t node_id = NodeIDNull,
                          NodeID_t parent_id = NodeIDNull,
                          const QString & name = QString());

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * conn) override;

    virtual NodeType_t type() const final override { return NODE_TYPE_USBISTREAM; }

protected:
    virtual void eventDestroy() override;

    virtual bool eventInit(DeviceController * controller) override;

    virtual bool eventSetup(const ControlPacket & packet) override;

    virtual void eventStreamToggled(bool enabled) override;

    virtual Length_t packetSize() const override { return _buffer.size(); }

private:
    inline void acquireUsedPacket() { _usedPacket.acquire(1); }

    inline void releaseFreePacket() { _freePacket.release(1); }

    inline uint32_t timeout() { return 10; } //! @todo

    void beginTransfer();

    void transferCplt(libusb_transfer * tfer);

    QSemaphore              _usedPacket;
    QSemaphore              _freePacket;
    QUsbIStreamProcessor *  _processor;
    libusb_device_handle *  _handle;
    uint8_t                 _epAddr;
    QByteArray              _buffer;
};

#endif // QUSBISTREAMNODE_H
