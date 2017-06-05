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

    inline bool isEnabled() const { return _enabled; }

    bool setEnabled(bool enabled);

    inline Length_t dataOffset() const { return _dataOffset; }

    inline Length_t dataLength() const { return _dataLength; }

    virtual void streamDataReceived(Data * data) = 0;

protected:
    virtual bool eventSetup(const ControlPacket & packet) override;

    virtual bool eventData(const ControlPacket & packet) override;

    virtual void toggledEvent(bool enabled) = 0;

    virtual bool eventInit(DeviceController * controller) override;

private:
    NodeID_t            _streamId;
    bool                _enabled;
    Length_t            _dataOffset;
    Length_t            _dataLength;

signals:
    void toggled(bool enable);
};

#endif // QOSTREAMCHANNELCONTROLLER_H
