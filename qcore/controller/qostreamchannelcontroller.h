#ifndef QOSTREAMCHANNELCONTROLLER_H
#define QOSTREAMCHANNELCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>

class QOStreamChannelController :
        public QNodeController
{
    Q_OBJECT
public:
    QOStreamChannelController(NodeID_t node_id,
                              NodeID_t parent_id,
                              const QString & name,
                              QDeviceConnection *dev);

    inline Length_t dataOffset() const { return _dataOffset; }

    inline Length_t dataLength() const { return _dataLength; }

    inline ParamDataIdx count() const { return _count; }

    virtual void streamDataReceived(Data * data) = 0;

protected:
    virtual bool eventSetup(const ControlPacket & packet) override;

    virtual void toggledEvent(bool enabled) { (void)enabled; }

    virtual bool eventInit(DeviceController * controller) override;

    virtual bool eventData(const ControlPacket & packet) override;

private:
    NodeID_t            _streamId;
    Length_t            _dataOffset;
    Length_t            _dataLength;
    ParamDataIdx        _count;

signals:
    void toggled(bool enable);
};

#endif // QOSTREAMCHANNELCONTROLLER_H
