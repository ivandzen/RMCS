#ifndef TCPBACKEND_H
#define TCPBACKEND_H
#include <controller/deviceconnection.h>
#include <QTcpSocket>
#include <QMutex>

class TcpBackend :
        public QObject,
        public DeviceConnection
{
    Q_OBJECT
public:
    explicit TcpBackend(QObject * parent = nullptr);

protected:
    bool init(qint64 timeout);

    virtual ControlPacket beginControlTransaction(Length_t max_length) override;

    virtual bool endControlTransaction(NodeID_t ep_id,
                                       CtlReq_t req,
                                       const ControlPacket & packet,
                                       Time_t timeout,
                                       const TransmitHandler & handler) override;

    virtual bool requestControlPacket(NodeID_t ep_id,
                                      CtlReq_t req,
                                      Length_t max_length,
                                      Time_t timeout,
                                      const ReceiveHandler & handler) override;

    virtual void timerEvent(QTimerEvent * event) override;

private:
    typedef uint8_t TransType_t;

    enum TransType
    {
        TT_INIT,
        TT_CTL_WRITE,
        TT_CTL_REQ
    };

    struct ControlHeader
    {
        TransType_t type;
        NodeID_t ep_id;
        CtlReq_t req;
        Length_t length;

        inline Length_t totalLength() const
        {
            return sizeof(ControlHeader) + length;
        }
    };

    inline ControlHeader * controlHeader() const
    {
        return reinterpret_cast<ControlHeader*>(const_cast<unsigned char*>(_controlPacket.data()));
    }

    inline unsigned char * controlData()
    {
        return const_cast<unsigned char*>(_controlPacket.data());
    }

    QTcpSocket *    _controlSocket;
    QTcpSocket *    _outStreamSocket;
    QTcpSocket *    _intStreamSocket;
    QMutex          _mutex;
    QVector<Data>   _controlPacket;
    int             _timerId;

private slots:
    void controlReadyRead();

    void bytesWritten(qint64);
};

#endif // TCPBACKEND_H
