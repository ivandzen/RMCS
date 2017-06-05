#ifndef QOSTREAMCONTROLLER_H
#define QOSTREAMCONTROLLER_H
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qostreamchannelcontroller.h>

class QOStreamController :
        public QNodeController
{
    Q_OBJECT
public:
    QOStreamController(NodeID_t node_id,
                       NodeID_t parent_id,
                       const QString & name,
                       QDeviceConnection *conn);

    inline bool isStreamEnabled() const { return _enabled; }

    bool setStreamEnabled(bool enabled);

    bool addChannel(QOStreamChannelController * channel);

    inline uint16_t numChannels() const {
        return uint16_t(_channels.size());
    }

    inline QOStreamChannelController * getChannel(uint16_t number) const {
        return _channels[number];
    }

    void processChannels(const ArrayRef<Data> & buffer);

    virtual NodeControllerForm * createForm(QWidget * parent = nullptr) override;

protected:
    virtual bool eventData(const ControlPacket & packet) override;

    virtual void eventStreamToggled(bool enabled) = 0;

private:
    bool _enabled;
    QVector<QOStreamChannelController *> _channels;

signals:
    void streamToggled(bool enabled);
};

#endif // QOSTREAMCONTROLLER_H
