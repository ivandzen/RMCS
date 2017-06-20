#ifndef ISTREAMNODE_H
#define ISTREAMNODE_H
#include <core/common/inputstream.h>
#include <core/device/node.h>
#include <core/device/properties.h>

class IStreamChannel;

class IStreamNode :
        public Node
{
    /// For every specific type of IStreamNode
    /// next methods must be implemented :
    ///     virtual bool settingsRequested(ControlPacket & packet)
    ///     virtual void streamToggled(bool enabled)
public:
    IStreamNode(const char * name,
                NodeType_t type,
                Length_t num_packets,
                Device * dev);

    bool addChannel(IStreamChannel * channel);

    inline Data * istreamPacket(Length_t idx) {
    	return _buffer.data() + _packetSize * idx;
    }

    inline Length_t numPackets() const { return _numPackets; }

    inline Length_t packetSize() const { return _packetSize; }

    inline bool isStreamEnabled() const { return _enabled; }

    void streamDataReceived(const Data * data,
        					Length_t length);

protected:
    virtual bool nodeDataRequested(ControlPacket & packet) const final override;

    virtual bool nodeDataReceived(const ControlPacket & packet) final override;

    virtual void streamToggled(bool enabled) = 0;

    virtual bool init() override;

private:
    BOOL_PROP(Enabled, false,
              [this]() { return _enabled; },
              [this](bool enabled) { return toggleStream(enabled); })

    bool toggleStream(bool enabled);

    bool        				_enabled;
    Length_t					_numPackets;
    Length_t    				_packetSize;
    std::vector<Data>			_buffer;
    std::vector<IStreamChannel*> _channels;
};

#endif // ISTREAMNODE_H
