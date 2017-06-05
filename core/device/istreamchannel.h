#ifndef ISTREAMCHANNEL_H
#define ISTREAMCHANNEL_H
#include <core/common/arrayref.h>
#include <core/device/istreamnode.h>

class IStreamNode;

class IStreamChannel :
        public Node
{
    friend class IStreamNode;
public:
    IStreamChannel(NodeType_t type,
                   const char * name,
                   IStreamNode * istream,
                   Length_t data_size,
                   Node * parent);

    IStreamChannel(NodeType_t type,
                   const char * name,
				   IStreamNode * istream,
                   Length_t data_size,
                   Device * dev);

    inline bool isEnabled() const { return _enabled; }

    inline Length_t dataLength() const { return _length; }

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool nodeDataRequested(ControlPacket & packet) const override;

    virtual bool nodeDataReceived(const ControlPacket & packet) override;

    virtual void streamDataReceived(const ArrayRef<Data> & data) = 0;

private:
    bool            _enabled;
    IStreamNode *   _istream;
    Length_t        _offset;
    Length_t        _length;
};

#endif // ISTREAMCHANNEL_H
