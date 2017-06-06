#ifndef QISTREAMCHANNELCONTROLLER_H
#define QISTREAMCHANNELCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>

class QIStreamChannelController :
        public QNodeController
{
    Q_OBJECT
public:
    QIStreamChannelController(NodeID_t node_id,
                              NodeID_t parent_id,
                              const QString & name,
                              QDeviceConnection *connection);

    inline bool isEnabled() const { return _enabled; }

    bool setEnabled(bool enabled);

    inline Length_t dataOffset() const { return _dataOffset; }

    inline Length_t dataLength() const { return _dataLength; }

    //virtual const Data * data() const = 0;

    virtual void fill(ArrayRef<Data> & arr) const = 0;

protected:
    virtual bool eventSetup(const ControlPacket & packet) override;

    virtual bool eventData(const ControlPacket & packet) override;

    virtual void toggledEvent(bool enabled) { (void)enabled; }

    virtual bool eventInit(DeviceController * controller) override;

private:
    NodeID_t            _streamId;
    bool                _enabled;
    Length_t            _dataOffset;
    Length_t            _dataLength;

signals:
    void toggled(bool enable);
};

#endif // QISTREAMCHANNELCONTROLLER_H
