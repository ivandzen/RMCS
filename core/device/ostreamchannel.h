#ifndef OSTREAMCHANNEL_H
#define OSTREAMCHANNEL_H
#include <core/common/arrayref.h>
#include <core/device/ostreamnode.h>

class OStreamNode;

class OStreamChannel :
        public Node
{
    friend class OStreamNode;
public:
    OStreamChannel(NodeType_t type,
                   const char * name,
                   OStreamNode * ostream,
                   Length_t data_size,
                   Node * parent);

    OStreamChannel(NodeType_t type,
                   const char * name,
                   OStreamNode * ostream,
                   Length_t data_size,
                   Device * dev);

    inline bool isEnabled() const { return _enabled; }

    inline Length_t dataLength() const { return _length; }

    inline void write(Data * data)
    {
        memcpy(_ostream->ostreamPacket() + _offset, data, _length);
    }

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool nodeDataRequested(ControlPacket & packet) const override;

    virtual bool nodeDataReceived(const ControlPacket & packet) override;

    inline Data * streamPacket() const { return _ostream->ostreamPacket() + _offset; }

private:
    OStreamNode *   _ostream;
    bool            _enabled;
    Length_t        _offset;
    Length_t        _length;
};

#endif // OSTREAMCHANNEL_H
