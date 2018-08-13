#ifndef USBBACKEND_H
#define USBBACKEND_H

#include <usb/common/common.h>
#include <core/common/control_protocol.h>
#include <qcore/controller/qdeviceconnection.h>
#include <qcore/controller/qostreamcontroller.h>
#include <qcore/controller/qistreamcontroller.h>
#include <libusb.h>
#include <QObject>
#include <QHash>
#include <QSemaphore>

///////////////////////////////////////////////////////////////////////////////////////////

class QDeviceController;

class QUsbConnection :
        public QDeviceConnection
{
    Q_OBJECT

    friend void CALLBACK_ATTRIB controlTransferCallback(struct libusb_transfer * transfer);
public:
    QUsbConnection(libusb_device_handle * handle,
                   QObject * parent = nullptr);

    void setTimeout(uint32_t tout) {
        _timeout = tout;
    }

    virtual ControlPacket beginCtlTransfer(Length_t max_length) override;

    virtual bool endCtlTransfer(NodeID_t node_id,
                                CtlReq_t type,
                                const ControlPacket & packet,
                                const TransmitHandler & handler) override;

    virtual void abortCtlTransfer() override;

    virtual bool requestCtlPacket(NodeID_t node_id,
                                  CtlReq_t type,
                                  Length_t max_length,
                                  const ReceiveHandler & handler) override;

    inline libusb_device_handle * handle() const { return _handle; }

protected:
    virtual QNodeController * createNode(NodeType_t node_type,
                                         NodeID_t node_id,
                                         NodeID_t parent_id,
                                         const QString & name) final override;

private:
    void logMessage(const QString & message);

    void transferCallback(libusb_transfer * transfer);

    inline ReceiveHandler rxHandler(libusb_transfer * transfer) {
        return _rx_transfers[transfer];
    }

    inline void removeRxHandler(libusb_transfer * transfer) {
        _rx_transfers.remove(transfer);
    }

    inline TransmitHandler txHandler(libusb_transfer * transfer) {
        return _tx_transfers[transfer];
    }

    inline void removeTxHandler(libusb_transfer * transfer) {
        _tx_transfers.remove(transfer);
    }

    QSemaphore  _semaphore;
    libusb_device_handle * _handle;
    QHash<libusb_transfer*, TransmitHandler> _tx_transfers;
    QHash<libusb_transfer*, ReceiveHandler> _rx_transfers;
    Data        _rx_buf[MAX_SETUP_DATA_LENGTH];
    Data        _tx_buf[MAX_SETUP_DATA_LENGTH];
    ControlPacket _rx_pack;
    ControlPacket _tx_pack;
    uint32_t    _timeout;
};

///////////////////////////////////////////////////////////////////////////////////////////

class QUsbOStreamController;

class QUSBOStreamReader :
        public QOStreamReader
{
    Q_OBJECT
    friend void CALLBACK_ATTRIB ostreamTransferCallback(libusb_transfer * transfer);

public:
    QUSBOStreamReader(UsbEPDescriptor::EPType_t type,
                      libusb_device_handle * handle,
                      uint8_t ep_addr,
                      QUsbOStreamController * controller);

private:
    virtual bool beginTransfer(char * ptr,
                               Length_t num_packets,
                               Length_t pack_size) final override;

    void transferCplt(struct libusb_transfer * tfer);

    struct libusb_transfer * _transfer;
    UsbEPDescriptor::EPType_t _type;
    libusb_device_handle *  _handle;
    uint8_t                 _epAddr;
};

///////////////////////////////////////////////////////////////////////////////////////////

class QUsbOStreamController :
        public QOStreamController
{
    Q_OBJECT
    friend class QOStreamProcessor;
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
    virtual bool eventSetup(const ControlPacket & packet) override;

private:
    inline unsigned int timeout() const { return PACKETS_PER_TRANSFER * 1000; }

    libusb_device_handle *  _handle;
};

///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////

class QUsbIStreamController :
        public QIStreamController
{
    Q_OBJECT
    friend class QUsbIStreamProcessor;
    friend void CALLBACK_ATTRIB istreamTransferCallback(libusb_transfer * transfer);
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

#endif // USBBACKEND_H
