#ifndef IPARAM_H
#define IPARAM_H
#include <core/common/control_protocol.h>
#include <core/device/device.h>
#include <core/device/istreamchannel.h>

template<typename T>
class IParam :
        public IStreamChannel
{
public:
    typedef std::function<void(T)> onRecvHandler;

    IParam(const onRecvHandler & handler,
           NodeType_t type,
           const char * name,
           IStreamNode * istream,
           Node * parent) :
        IStreamChannel(type, name, istream, sizeof(ParamData<T>), parent),
        _lastData({ 0xFF, T() }),
        _handler(handler)
    {}

    IParam(const onRecvHandler & handler,
           NodeType_t type,
           const char * name,
           IStreamNode * istream,
           Device * device) :
        IStreamChannel(type, name, istream, sizeof(ParamData<T>), device),
        _lastData({ 0xFF, T() }),
        _handler(handler)
    {}

    inline T value() const { return _lastData.value; }

    inline void operator >> (T & target) const { target = value(); }

private:
    inline bool needUpdate(const ArrayRef<Data> & data) const
    {
        return reinterpret_cast<const ParamData<T>*>(data.data())->id != _lastData.id;
    }

    virtual void streamDataReceived(const ArrayRef<Data> & data) override
    {
        if(needUpdate(data))
        {
            data.copyTo(reinterpret_cast<Data*>(&_lastData));
            _handler(_lastData.value);
        }
    }

    ParamData<T>    _lastData;
    onRecvHandler   _handler;
};

#endif // IPARAM_H
