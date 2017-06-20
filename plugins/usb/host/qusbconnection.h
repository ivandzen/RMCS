#ifndef QUSBCONNECTION_H
#define QUSBCONNECTION_H
#include <qcore/controller/qdeviceconnection.h>
#include <usb/common/common.h>
#include <core/common/control_protocol.h>
#include <libusb.h>
#include <QObject>
#include <QHash>
#include <QSemaphore>

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

#endif // QUSBCONNECTION_H
