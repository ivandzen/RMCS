#ifndef OPARAM_H
#define OPARAM_H
#include <core/common/control_protocol.h>
#include <core/device/device.h>
#include <core/device/ostreamchannel.h>

template<typename T>
class OParam :
        public OStreamChannel
{
public:
    typedef std::function<void(T)> onRecvHandler;

    OParam(NodeType_t type,
           const char * name,
           OStreamNode * ostream,
           Node * parent) :
        OStreamChannel(type, name, ostream, sizeof(ParamData<T>), parent),
        _lastData({ 0xFF, T() })
    {}

    OParam(NodeType_t type,
           const char * name,
           OStreamNode * ostream,
           Device * device) :
        OStreamChannel(type, name, ostream, sizeof(ParamData<T>), device),
        _lastData({ 0xFF, T() })
    {}

    inline void write(T value)
    {
        ++_lastData.id;
        _lastData.value = value;
        OStreamChannel::write(reinterpret_cast<Data*>(&_lastData));
    }

    inline OParam & operator << (T value)
    {
        write(ArrayRef<Data>(reinterpret_cast<Data*>(&value), sizeof (value)));
        return *this;
    }

private:
    ParamData<T>   _lastData;
};


#endif // OPARAM_H
