#ifndef QUSBBULKOSTREAMCONTROLLER_H
#define QUSBBULKOSTREAMCONTROLLER_H
#include <qcore/controller/qostreamcontroller.h>
#include <usb/common/common.h>
#include <libusb.h>

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

///////////////////////////////////////////////////////////////////////

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

#endif // QUSBBULKOSTREAMCONTROLLER_H
