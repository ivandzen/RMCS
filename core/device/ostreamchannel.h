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

    inline Length_t dataLength() const { return _length; }

    inline Data * streamPtr() const { return _ostream->ostreamPacket() + _offset; }

    inline void write(Data * data)
    {
        memcpy(streamPtr(), data, _length);
    }

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

private:
    OStreamNode *   _ostream;
    Length_t        _offset;
    Length_t        _length;
};

#endif // OSTREAMCHANNEL_H
