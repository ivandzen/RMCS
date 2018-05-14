#ifndef RAWOCHANNEL_H
#define RAWOCHANNEL_H
#include <core/common/control_protocol.h>
#include <core/device/device.h>
#include <core/device/ostreamchannel.h>

template<typename T, size_t buffer_size>
class RawOChannel :
        public OStreamChannel
{
public:
    RawOChannel(NodeType_t type,
                const char * name,
                OStreamNode * ostream,
                Node * parent) :
        OStreamChannel(type, name, ostream, sizeof(T) * buffer_size, parent)
    {}

    RawOChannel(NodeType_t type,
                const char * name,
                OStreamNode * ostream,
                Device * device) :
        OStreamChannel(type, name, ostream, sizeof(T) * buffer_size, device)
    {}

    inline bool set(T value, size_t index) {
        memcpy(streamPtr() + sizeof(T) * index, &value, sizeof(T));
        return true;
    }
};
#endif // RAWOCHANNEL_H
