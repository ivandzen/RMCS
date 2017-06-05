#ifndef ENDPOINTS_H
#define ENDPOINTS_H
#include <core/device/iparam.h>
#include <core/device/oparam.h>
#include <core/device/property.h>
#include <core/common/defaultnodes.h>

class UInt8IParam :
		public IParam<uint8_t>
{
public:
	UInt8IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
					IParam<uint8_t>(handler, NODE_TYPE_UINT8_IPARAM, name, istream, parent)
	{}
};

class UInt16IParam :
		public IParam<uint16_t>
{
public:
	UInt16IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
					IParam<uint16_t>(handler, NODE_TYPE_UINT16_IPARAM, name, istream, parent)
	{}
};

class UInt32IParam :
		public IParam<uint32_t>
{
public:
	UInt32IParam(const char * name,
				IStreamNode * istream,
				const onRecvHandler & handler,
				Node * parent) :
					IParam<uint32_t>(handler, NODE_TYPE_UINT32_IPARAM, name, istream, parent)
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

class UInt8OParam :
		public OParam<uint8_t>
{
public:
	UInt8OParam(const char * name,
				OStreamNode * ostream,
				Node * parent) :
					OParam<uint8_t>(NODE_TYPE_UINT8_OPARAM, name, ostream, parent)
	{}
};

class UInt16OParam :
		public OParam<uint16_t>
{
public:
	UInt16OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
					OParam<uint16_t>(NODE_TYPE_UINT16_OPARAM, name, ostream, parent)
	{}
};

class UInt32OParam :
		public OParam<uint32_t>
{
public:
	UInt32OParam(const char * name,
				 OStreamNode * ostream,
				 Node * parent) :
					 OParam<uint32_t>(NODE_TYPE_UINT32_OPARAM, name, ostream, parent)
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
					  OParam<double>(NODE_TYPE_FLOAT64_OPARAM, name, ostream, parent)
	{}
};

///////////////////////////////////////////////////////////////////////////////////

#define BOOL_PROP_RDONLY(name, getter) \
    ROProperty<bool> name = ROProperty<bool>(NODE_TYPE_BOOL_PROP_RDONLY, \
                                             #name, this, getter);

#define INT_PROP_RDONLY(name, getter) \
    ROProperty<int32_t> name = ROProperty<int32_t>(NODE_TYPE_INT_PROP_RDONLY, \
                                                   #name, this, getter);

#define FLOAT_PROP_RDONLY(name, getter) \
    ROProperty<float> name = ROProperty<float>(NODE_TYPE_FLOAT_PROP_RDONLY, \
                                               #name, this, getter);

///////////////////////////////////////////////////////////////////////////////////

#define BOOL_PROP(name, default_value, getter, setter) \
    RWProperty<bool> name = RWProperty<bool>(NODE_TYPE_BOOL_PROP, \
                                             #name, this, \
                                             getter, setter, \
                                             default_value);

#define INT_PROP(name, default_value, getter, setter) \
    RWProperty<int32_t> name = RWProperty<int32_t>(NODE_TYPE_INT_PROP, \
                                                   #name, this, \
                                                   getter, setter, \
                                                   default_value);

#define FLOAT_PROP(name, default_value, getter, setter) \
    RWProperty<float> name = RWProperty<float>(NODE_TYPE_FLOAT_PROP, \
                                               #name, this, \
                                               getter, setter, \
                                               default_value);


#endif // ENDPOINTS_H
