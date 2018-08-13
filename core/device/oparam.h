#ifndef OPARAM_H
#define OPARAM_H
#include <core/common/control_protocol.h>
#include <core/device/device.h>
#include <core/device/ostreamchannel.h>

template<typename T, uint8_t count>
class OParam :
        public OStreamChannel
{
public:
	typedef _ParamData<T> PData;
	typedef typename PData::IDType PDataID;
	typedef typename PData::IdxType PDataIdx;

    OParam(NodeType_t type,
           const char * name,
           OStreamNode * ostream,
           Node * parent) :
        OStreamChannel(type, name, ostream, PData::byteSize(count), parent),
		_data(PData(streamPtr(), count)),
		_curIndex(0)
    {}

    OParam(NodeType_t type,
           const char * name,
           OStreamNode * ostream,
           Device * device) :
        OStreamChannel(type, name, ostream, PData::byteSize(count), device),
		_data(PData(streamPtr(), count)),
		_curIndex(0)
    {}

    inline void write(PDataIdx idx, T value) {
    	_data.set(idx, value);
    }

    inline void reset() { _curIndex = 0; }

    inline void submit(PDataID id) { _data.setID(id); }

    inline OParam<T, count> & operator<<(T value) {
    	write(_curIndex, value);
    	if(++_curIndex >= count)
    		_curIndex = 0;
    	return *this;
    }

protected:
    virtual bool nodeDataRequested(ControlPacket & dpack) const override
	{
    	DataPacket<PDataIdx> sizepack(dpack);
    	return sizepack.init(count);
	}

    virtual bool nodeDataReceived(const ControlPacket & dpack) override
    {
    	(void)dpack;
    	return false;
    }

private:
    PData		_data;
    PDataIdx	_curIndex;
};

#endif // OPARAM_H
