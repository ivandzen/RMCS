#ifndef ENDPOINTS_H
#define ENDPOINTS_H
#include <core/device/iparam.h>
#include <core/device/oparam.h>
#include <core/device/rawichannel.h>
#include <core/device/rawochannel.h>


class Int8IParam :
        public IParam<int8_t>
{
public:
	Int8IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int8_t>(handler, NODE_TYPE_INT8_IPARAM, name, istream, parent)
	{}
};

class Int16IParam :
        public IParam<int16_t>
{
public:
	Int16IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int16_t>(handler, NODE_TYPE_INT16_IPARAM, name, istream, parent)
	{}
};

class Int32IParam :
        public IParam<int32_t>
{
public:
	Int32IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
                    IParam<int32_t>(handler, NODE_TYPE_INT32_IPARAM, name, istream, parent)
	{}
};

class Float32IParam :
		public IParam<float>
{
public:
	Float32IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Node * parent) :
					  IParam<float>(handler, NODE_TYPE_FLOAT32_IPARAM, name, istream, parent)
	{}
};

class Float64IParam :
		public IParam<double>
{
public:
	Float64IParam(const char * name,
				  IStreamNode * istream,
				  const onRecvHandler & handler,
				  Node * parent) :
					  IParam<double>(handler, NODE_TYPE_FLOAT64_IPARAM, name, istream, parent)
	{}
};

///////////////////////////////////////////////////////////////////////////////////

class Int8OParam :
        public OParam<int8_t>
{
public:
	Int8OParam(const char * name,
				OStreamNode * ostream,
				Node * parent) :
                    OParam<int8_t>(NODE_TYPE_INT8_OPARAM, name, ostream, parent)
	{}
};

class Int16OParam :
        public OParam<int16_t>
{
public:
	Int16OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
                    OParam<int16_t>(NODE_TYPE_INT16_OPARAM, name, ostream, parent)
	{}
};

class Int32OParam :
        public OParam<int32_t>
{
public:
	Int32OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
                     OParam<int32_t>(NODE_TYPE_INT32_OPARAM, name, ostream, parent)
	{}
};

class Float32OParam :
		public OParam<float>
{
public:
	Float32OParam(const char * name,
				  OStreamNode * ostream,
                  Node * parent) :
        OParam<float>(NODE_TYPE_FLOAT32_OPARAM, name, ostream, parent)
	{}
};

class Float64OParam :
		public OParam<double>
{
public:
	Float64OParam(const char * name,
				  OStreamNode * ostream,
                  Node * parent) :
        OParam<double>(NODE_TYPE_FLOAT64_OPARAM,
                       name,
                       ostream,
                       parent)
	{}
};

///////////////////////////////////////////////////////////////////////////////////

template <size_t buffer_size>
class RawInt8OChannel :
        public RawOChannel<int8_t, buffer_size>
{
public:
    RawInt8OChannel(const char * name,
                    OStreamNode * ostream,
                    Node * parent) :
        RawOChannel<uint8_t, buffer_size>(NODE_TYPE_RAW_INT8_OCHANNEL,
                                          name,
                                          ostream,
                                          parent)
    {}
};

template <size_t buffer_size>
class RawInt16OChannel :
        public RawOChannel<int16_t, buffer_size>
{
public:
    RawInt16OChannel(const char * name,
                     OStreamNode * ostream,
                     Node * parent) :
        RawOChannel<uint16_t, buffer_size>(NODE_TYPE_RAW_INT16_OCHANNEL,
                                           name,
                                           ostream,
                                           parent)
    {}
};

template <size_t buffer_size>
class RawInt32OChannel :
        public RawOChannel<int32_t, buffer_size>
{
public:
    RawInt32OChannel(const char * name,
                     OStreamNode * ostream,
                     Node * parent) :
        RawOChannel<uint32_t, buffer_size>(NODE_TYPE_RAW_INT32_OCHANNEL,
                                           name,
                                           ostream,
                                           parent)
    {}
};

#endif // ENDPOINTS_H
