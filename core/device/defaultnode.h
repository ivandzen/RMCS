#ifndef DEFAULTNODE_H
#define DEFAULTNODE_H
#include <core/device/node.h>

class DefaultNode :
        public Node
{
public:
    DefaultNode(const char * name,
                Node * parent);

    DefaultNode(const char * name,
                Device * device);

protected:
    virtual bool settingsRequested(ControlPacket & packet) const override;

    virtual bool nodeDataRequested(ControlPacket &packet) const override;

    virtual bool nodeDataReceived(const ControlPacket &) override;
};

#endif // DEFAULTNODE_H
