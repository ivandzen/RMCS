#ifndef OSTREAMNODE_H
#define OSTREAMNODE_H
#include <core/common/outputstream.h>
#include <core/device/node.h>
#include <core/device/properties.h>

class OStreamChannel;

class OStreamNode :
        public Node
{
public:
    OStreamNode(const char * name,
                NodeType_t type,
                Device * dev,
				uint16_t bufferSize);

    bool addChannel(OStreamChannel * channel);

    inline bool isStreamEnabled() const { return _enabled; }

    inline Length_t bufferSize() const { return _buffer.size(); }

    inline Data * ostreamPacket() { return _buffer.data(); }

    virtual void sync() = 0;

protected:
    virtual bool nodeDataRequested(ControlPacket & packet) const override;

    virtual bool nodeDataReceived(const ControlPacket & packet) override;

    virtual void streamToggled(bool enabled) = 0;

private:
    bool toggleStream(bool enabled);

    BOOL_PROP(Enabled, this, false,
              [this]() { return _enabled; },
              [this](bool enabled) { return toggleStream(enabled); })

    bool 	_enabled;
    Length_t _packetSize;
    std::vector<Data> _buffer;
};

#endif // OSTREAMNODE_H
