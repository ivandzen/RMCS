#ifndef QISTREAMCONTROLLER_H
#define QISTREAMCONTROLLER_H
#include <QVector>
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qistreamchannelcontroller.h>

class QRWPropertyController;

class QIStreamController :
        public QNodeController
{
    Q_OBJECT
public:
    QIStreamController(NodeID_t node_id,
                       NodeID_t parent_id,
                       const QString & name,
                       QDeviceConnection * connection);

    bool isStreamEnabled() const;

    bool setStreamEnabled(bool enabled);

    bool addChannel(QIStreamChannelController * channel);

    inline uint16_t numChannels() const {
        return uint16_t(_channels.size());
    }

    inline QIStreamChannelController * getChannel(uint16_t number) const {
        return _channels[number];
    }

    void processChannels(const ArrayRef<Data> & buffer);

protected:
    virtual bool eventInit(DeviceController * controller) override;

    virtual bool eventData(const ControlPacket & packet) override;

    virtual void eventStreamToggled(bool enabled) = 0;

    virtual Length_t packetSize() const = 0;

private:
    QVector<QIStreamChannelController *> _channels;
    QRWPropertyController *              _enabled;
};

#endif // QISTREAMCONTROLLER_H
