#ifndef QOSTREAMCONTROLLER_H
#define QOSTREAMCONTROLLER_H
#include <QVector>
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qostreamchannelcontroller.h>

class QRWPropertyController;

class QOStreamController :
        public QNodeController
{
    Q_OBJECT
public:
    QOStreamController(NodeID_t node_id,
                       NodeID_t parent_id,
                       const QString & name,
                       QDeviceConnection *conn);

    bool isStreamEnabled() const;

    bool setStreamEnabled(bool enabled);

    bool addChannel(QOStreamChannelController * channel);

    inline uint16_t numChannels() const {
        return uint16_t(_channels.size());
    }

    inline QOStreamChannelController * getChannel(uint16_t number) const {
        return _channels[number];
    }

    void processChannels(const ArrayRef<Data> & buffer);

protected:
    virtual bool eventInit(DeviceController * device) override;

    virtual bool eventData(const ControlPacket & packet) override;

    virtual void eventStreamToggled(bool enabled) = 0;

private:
    QRWPropertyController *             _enabled;
    QVector<QOStreamChannelController *> _channels;
};

#endif // QOSTREAMCONTROLLER_H
