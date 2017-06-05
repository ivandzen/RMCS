#ifndef PROPERTY_H
#define PROPERTY_H
#include <core/common/control_protocol.h>
#include <core/device/node.h>

////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class ROProperty :
        public Node
{
public:
    typedef std::function<T(void)> Getter;

    ROProperty(NodeType_t type,
               const char * name,
               Node * parent,
               const Getter & getter) :
        Node(type, name, parent),
        _getter(getter)
    {}

    ROProperty(NodeType_t type,
             const char * name,
             Device * device,
             const Getter & getter) :
        Node(type, name, device),
        _getter(getter)
    {}

    inline operator T() const { return _getter(); }

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override
	{
    	return packet.init(0);
	}

    virtual bool nodeDataRequested(ControlPacket & packet) const override
    {
        DataPacket<T> data(packet);
        if(data.init(_getter()))
            return true;
        //printLog("nodeDataRequested : Unable to init data packet");
        return false;
    }

    virtual bool nodeDataReceived(const ControlPacket &) override
    {
    	return true;
    }

private:
    Getter _getter;
};

////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class RWProperty :
        public ROProperty<T>
{
public:
    typedef std::function<bool(T)> Setter;

    RWProperty(NodeType_t type,
               const char * name,
               Node * parent,
               const typename ROProperty<T>::Getter & getter,
               const Setter & setter,
               T default_value) :
        ROProperty<T>(type, name, parent, getter),
        _setter(setter),
        _default(default_value)
    {}

    RWProperty(NodeType_t type,
               const char * name,
               Device * device,
               const typename ROProperty<T>::Getter & getter,
               const Setter & setter,
               T default_value) :
        ROProperty<T>(type, name, device, getter),
        _setter(setter),
        _default(default_value)
    {}

protected:
    virtual bool nodeDataReceived(const ControlPacket & packet) override
    {
        DataPacket<T> data(packet);
        if(!data.isValid())
        {
            //ROProperty<T>::printLog("nodeDataReceived : invalid data packet");
            return  false;
        }

        return _setter(*data.get());
    }

    virtual bool init() override
    {
        if(!ROProperty<T>::init())
        	return false;
        return _setter(_default);
    }

private:
    Setter  _setter;
    T       _default;
};

#endif // PROPERTY_H
