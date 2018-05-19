#ifndef IPARAM_H
#define IPARAM_H
#include <core/common/control_protocol.h>
#include <core/device/device.h>
#include <core/device/istreamchannel.h>

template<typename T, uint8_t count>
class IParam :
        public IStreamChannel
{
public:
    typedef std::function<void(const _ParamData<T> & )> onRecvHandler;

    typedef _ParamData<T>	PData;
    typedef typename PData::IDType	PDataID;
    typedef typename PData::IdxType	PDataIdx;

    IParam(const onRecvHandler & handler,
           NodeType_t type,
           const char * name,
           IStreamNode * istream,
           Node * parent) :
        IStreamChannel(type, name, istream, PData::byteSize(count), parent),
        _handler(handler)
    {}

    IParam(const onRecvHandler & handler,
           NodeType_t type,
           const char * name,
           IStreamNode * istream,
           Device * device) :
        IStreamChannel(type, name, istream, PData::byteSize(count), device),
        _handler(handler)
    {}

    inline T value(PDataIdx idx) const { return _data.get(idx); }

private:
    virtual void streamDataReceived(const ArrayRef<Data> & data) override
    {
    	_data = PData(data.data(), count);
        if(_data.getID() != _lastId)
        {
            _handler(_data);
            _lastId = _data.getID();
        }
    }

    onRecvHandler  	_handler;
    PData			_data;
    PDataID			_lastId;
};

#endif // IPARAM_H
