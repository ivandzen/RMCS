#ifndef COMMON_H
#define COMMON_H
#include <QHostAddress>
#include <core/common/control_protocol.h>

const NodeType_t NODE_TYPE_TCP_INPUT_STREAM = 35;

class TcpIStreamSettings :
        public ControlPacket
{
public:
    explicit TcpIStreamSettings(const ControlPacket & other) :
        ControlPacket (other)
    {}

    inline bool isValid() const
    {
        return ControlPacket::isValid() &&
               (payloadLength() >= sizeof(TcpInputStream_DH)) &&
               (payloadLength() == (sizeof(TcpInputStream_DH) +
                                    header()->addr_length));
    }

    inline bool init(const Time_t period,
                     const Length_t packet_size,
                     const quint16 port,
                     const QHostAddress & address)
    {
        QString addr = address.toString();
        if(!ControlPacket::init(sizeof(TcpInputStream_DH) + addr.size()))
            return false;
        header()->period = period;
        header()->packet_size = packet_size;
        header()->port = port;
        header()->addr_length = uint8_t(addr.size());
        memcpy(header()->addr,
               addr.toStdString().data(),
               size_t(addr.size()));
        return true;
    }

    inline Time_t period() const
    {
        return header()->period;
    }

    inline Length_t packetSize() const
    {
        return header()->packet_size;
    }

    inline QHostAddress address() const
    {
        return QHostAddress(QString(QByteArray(header()->addr,
                                               header()->addr_length)));
    }

    inline quint16 port() const
    {
        return header()->port;
    }

private:
    struct TcpInputStream_DH
    {
        Time_t      period;
        Length_t packet_size;
        uint16_t port;
        uint8_t addr_length;
        char addr[];
    };

    inline TcpInputStream_DH * header() const
    {
        return reinterpret_cast<TcpInputStream_DH*>(ControlPacket::payload());
    }
};

#endif // COMMON_H
