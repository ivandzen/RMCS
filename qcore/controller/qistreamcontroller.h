#ifndef QISTREAMCONTROLLER_H
#define QISTREAMCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qistreamchannelcontroller.h>

class QIStreamController :
        public QNodeController
{
    Q_OBJECT
public:
    QIStreamController(NodeID_t node_id,
                       NodeID_t parent_id,
                       const QString & name,
                       QDeviceConnection * connection);

    inline bool isStreamEnabled() const { return _enabled; }

    bool setStreamEnabled(bool enabled);

    bool addChannel(QIStreamChannelController * channel);

    inline uint16_t numChannels() const {
        return uint16_t(_channels.size());
    }

    inline QIStreamChannelController * getChannel(uint16_t number) const {
        return _channels[number];
    }

    void processChannels(const ArrayRef<Data> & buffer);

    virtual NodeControllerForm * createForm(QWidget * parent = nullptr) override;

protected:
    virtual bool eventData(const ControlPacket & packet) override;

    virtual void eventStreamToggled(bool enabled) = 0;

    virtual Length_t packetSize() const = 0;

private:
    bool _enabled;
    QVector<QIStreamChannelController *> _channels;

signals:
    void streamToggled(bool enabled);
};

#endif // QISTREAMCONTROLLER_H
