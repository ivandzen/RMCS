#ifndef ENDPOINTS_H
#define ENDPOINTS_H
#include <core/device/iparam.h>
#include <core/device/oparam.h>

template<uint8_t count>
class Int8IParam :
        public IParam<int8_t, count>
{
public:
	typedef typename IParam<int8_t, count>::onRecvHandler onRecvHandler;

	Int8IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int8_t, count>(handler, NODE_TYPE_INT8_IPARAM, name, istream, parent)
	{}

	Int8IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Device * dev) :
                    IParam<int8_t, count>(handler, NODE_TYPE_INT8_IPARAM, name, istream, dev)
	{}
};

template<uint8_t count>
class Int16IParam :
        public IParam<int16_t, count>
{
public:
	typedef typename IParam<int8_t, count>::onRecvHandler onRecvHandler;

	Int16IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int16_t, count>(handler, NODE_TYPE_INT16_IPARAM, name, istream, parent)
	{}

	Int16IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Device * dev) :
                    IParam<int16_t, count>(handler, NODE_TYPE_INT16_IPARAM, name, istream, dev)
	{}
};

template<uint8_t count>
class Int32IParam :
        public IParam<int32_t, count>
{
public:
	typedef typename IParam<int8_t, count>::onRecvHandler onRecvHandler;

	Int32IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int32_t, count>(handler, NODE_TYPE_INT32_IPARAM, name, istream, parent)
	{}

	Int32IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Device * dev) :
                    IParam<int32_t, count>(handler, NODE_TYPE_INT32_IPARAM, name, istream, dev)
	{}
};

template<uint8_t count>
class Float32IParam :
		public IParam<float, count>
{
public:
	typedef typename IParam<int8_t, count>::onRecvHandler onRecvHandler;

	Float32IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Node * parent) :
					  IParam<float, count>(handler, NODE_TYPE_FLOAT32_IPARAM, name, istream, parent)
	{}

	Float32IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Device * dev) :
					  IParam<float, count>(handler, NODE_TYPE_FLOAT32_IPARAM, name, istream, dev)
	{}
};

template<uint8_t count>
class Float64IParam :
		public IParam<double, count>
{
public:
	typedef typename IParam<int8_t, count>::onRecvHandler onRecvHandler;

	Float64IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Node * parent) :
					  IParam<double, count>(handler, NODE_TYPE_FLOAT64_IPARAM, name, istream, parent)
	{}

	Float64IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Device * dev) :
					  IParam<double, count>(handler, NODE_TYPE_FLOAT64_IPARAM, name, istream, dev)
	{}
};

///////////////////////////////////////////////////////////////////////////////////

template<uint8_t size>
class Int8OParam :
        public OParam<int8_t, size>
{
public:
	Int8OParam(const char * name,
				OStreamNode * ostream,
				Node * parent) :
                    OParam<int8_t, size>(NODE_TYPE_INT8_OPARAM, name, ostream, parent)
	{}

	Int8OParam(const char * name,
				OStreamNode * ostream,
				Device * dev) :
                    OParam<int8_t, size>(NODE_TYPE_INT8_OPARAM, name, ostream, dev)
	{}
};

template<uint8_t count>
class Int16OParam :
        public OParam<int16_t, count>
{
public:
	Int16OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
                    OParam<int16_t, count>(NODE_TYPE_INT16_OPARAM, name, ostream, parent)
	{}

	Int16OParam(const char * name,
				 OStreamNode * ostream,
				 Device * dev) :
                    OParam<int16_t, count>(NODE_TYPE_INT16_OPARAM, name, ostream, dev)
	{}
};

template<uint8_t count>
class Int32OParam :
        public OParam<int32_t, count>
{
public:
	Int32OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
                     OParam<int32_t, count>(NODE_TYPE_INT32_OPARAM, name, ostream, parent)
	{}

	Int32OParam(const char * name,
				OStreamNode * ostream,
				Device * dev) :
	                     OParam<int32_t, count>(NODE_TYPE_INT32_OPARAM, name, ostream, dev)
		{}
};

template<uint8_t count>
class Float32OParam :
		public OParam<float, count>
{
public:
	Float32OParam(const char * name,
				  OStreamNode * ostream,
                  Node * parent) :
        OParam<float, count>(NODE_TYPE_FLOAT32_OPARAM, name, ostream, parent)
	{}

	Float32OParam(const char * name,
				  OStreamNode * ostream,
	              Device * dev) :
	        OParam<float, count>(NODE_TYPE_FLOAT32_OPARAM, name, ostream, dev)
		{}
};

template<uint8_t count>
class Float64OParam :
		public OParam<double, count>
{
public:
	Float64OParam(const char * name,
				  OStreamNode * ostream,
                  Node * parent) :
        OParam<double, count>(NODE_TYPE_FLOAT64_OPARAM,
                       name,
                       ostream,
                       parent)
	{}

	Float64OParam(const char * name,
				  OStreamNode * ostream,
	              Device * dev) :
	    OParam<double, count>(NODE_TYPE_FLOAT64_OPARAM,
	                   name,
	                   ostream,
	                   dev)
		{}
};

#endif // ENDPOINTS_H
