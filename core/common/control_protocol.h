#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H
#include <core/common/arrayref.h>

#pragma pack(push, 1)

enum CtlRequest
{
    CTLREQ_NONE,
    CTLREQ_DEV_INFO,
    CTLREQ_DEV_DESCRIPTOR,
    CTLREQ_NODE_DESCRIPTOR,
    CTLREQ_NODE_SETTINGS,
    CTLREQ_NODE_STATUS,
    CTLREQ_NODE_DATA,
    CTLREQ_NODE_RESET
};

//////////////////////////////////////////////////////////////////////////////////////

class ControlPacket :
        public ArrayRef<Data>
{
public:
    ControlPacket(Data * data, Length_t size) :
        ArrayRef<Data> (data, size)
    {
        //header()->payloadLength = 0;
    }

    explicit ControlPacket(const ArrayRef<Data> & other) :
        ArrayRef<Data> (other)
    {
        //header()->payloadLength = 0;
    }

    ControlPacket(const ControlPacket & other) :
        ArrayRef<Data>(other)
    {}

    inline bool isValid() const
    {
        return (payloadLength() < size());
    }

    inline bool init(Length_t payload_length)
    {
        if(payload_length > size())
            return false;
        header()->payloadLength = payload_length;
        return true;
    }

    inline Length_t payloadLength() const { return header()->payloadLength; }

    inline Data * payload() const { return header()->payload; }

    inline Length_t packetSize() const { return sizeof(Header) + payloadLength(); }

    typedef struct
    {
        uint16_t payloadLength;
        Data payload[];
    } Header;

private:
    inline Header * header() const { return reinterpret_cast<Header*>(data()); }

    /*
    inline uint8_t createSA(uint8_t sa_len)
    {
        if(payloadLength() + sa_len > size())
            return false;
        header()->payloadLength += sa_len;
        return sa_len;
    }
    */
};

///////////////////////////////////////////////////////////////////////////////////

class NodeDescriptor :
        public ControlPacket
{
public:
    NodeDescriptor(const ControlPacket & other) :
        ControlPacket(other)
    {}

    inline bool isValid() const
    {
        return ControlPacket::isValid() &&
               (payloadLength() > sizeof(BaseNode_DH)) &&
               (payloadLength() == sizeof(BaseNode_DH) + nameLength());
    }

    inline bool init(NodeID_t id,
                     NodeID_t parent_id,
                     NodeType_t nodeType,
                     const char * node_name)
    {
        uint8_t name_len = uint8_t(strlen(node_name));
        if(!ControlPacket::init(sizeof(BaseNode_DH) + name_len))
            return false;
        header()->id = id;
        header()->parent_id = parent_id;
        header()->nodeType = nodeType;
        header()->name_length = name_len;
        memcpy(name(), node_name, name_len);
        return true;
    }

    inline NodeID_t id() const { return header()->id; }

    inline NodeID_t parent_id() const { return header()->parent_id; }

    inline NodeType_t nodeType() const { return header()->nodeType; }

    inline uint8_t nameLength() const { return header()->name_length; }

    inline const char * name() const {
        return header()->name;
    }

    inline char * name() {
        return header()->name;
    }

protected:
    typedef struct
    {
        NodeID_t id;
        NodeID_t parent_id;
        NodeType_t nodeType;
        uint8_t name_length;
        char name[];
    } BaseNode_DH;

    inline BaseNode_DH * header() const
    {
        return reinterpret_cast<BaseNode_DH*>(ControlPacket::payload());
    }
};

///////////////////////////////////////////////////////////////////////////////////

template<typename DataType>
class DataPacket :
        public ControlPacket
{
public:
    DataPacket(const ControlPacket & other) :
        ControlPacket(other)
    {}

    inline bool isValid() const
    {
        return ControlPacket::isValid() &&
               (payloadLength() == sizeof(DataType));
    }

    inline bool init(DataType d)
    {
        if(!ControlPacket::init(sizeof(DataType)))
            return false;
        memcpy(payload(), &d, sizeof(DataType));
        return true;
    }

    inline DataType * get() const
    {
        return reinterpret_cast<DataType*>(ControlPacket::payload());
    }
};

///////////////////////////////////////////////////////////////////////////////////

class StreamChannelSettings :
        public ControlPacket
{
public:
    StreamChannelSettings(const ControlPacket & other) :
        ControlPacket(other)
    {}

    inline bool isValid() const
    {
        return ControlPacket::isValid() &&
               (payloadLength() == sizeof(StreamNode_DH));
    }

    inline bool init(NodeID_t stream_id,
                     uint16_t data_offset,
                     uint16_t data_len)
    {
        if(!ControlPacket::init(sizeof(StreamNode_DH)))
            return false;
        header()->stream_id = stream_id;
        header()->dataOffset = data_offset;
        header()->dataLen = data_len;
        return true;
    }

    inline NodeID_t streamID() const { return header()->stream_id; }

    inline Length_t dataOffset() const { return header()->dataOffset; }

    inline Length_t dataLength() const { return header()->dataLen; }

private:
    typedef struct
    {
        NodeID_t 	stream_id;
        Length_t    dataOffset;
        Length_t    dataLen;
    } StreamNode_DH;

    inline StreamNode_DH * header() const
    {
        return reinterpret_cast<StreamNode_DH*>(ControlPacket::payload());
    }
};

///////////////////////////////////////////////////////////////////////////////////

class DeviceDescriptor :
        public ControlPacket
{
public:
    typedef struct
    {
        uint16_t numNodes;
        Length_t maxCtlPackLength;
    } DeviceDH;

    DeviceDescriptor(const ControlPacket & other) :
        ControlPacket(other)
    {}

    inline bool isValid() const
    {
        return ControlPacket::isValid() &&
               (payloadLength() == sizeof(DeviceDH));
    }

    inline bool init(size_t num_nodes,
                     Length_t max_ctl_pack_len)
    {
        if(!ControlPacket::init(sizeof(DeviceDH)))
            return false;
        header()->numNodes = num_nodes;
        header()->maxCtlPackLength = max_ctl_pack_len;
        return true;
    }

    inline size_t numNodes() const
    {
        return header()->numNodes;
    }

    inline Length_t maxCtlPackLength() const
    {
        return header()->maxCtlPackLength;
    }

private:
    inline DeviceDH * header() const
    {
        return reinterpret_cast<DeviceDH*>(ControlPacket::payload());
    }

};

#pragma pack(pop)

#endif // CONTROL_PROTOCOL_H
