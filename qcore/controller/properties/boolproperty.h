#ifndef BOOLPROPERTY_H
#define BOOLPROPERTY_H
#include <qcore/controller/qnodecontroller.h>

class BoolProperty :
        public QNodeController
{
    Q_OBJECT
public:
    BoolProperty(NodeID_t node_id,
                 NodeID_t parent_id,
                 const QString & name,
                 QDeviceConnection * dev);

    virtual QNodeController * createInstance(NodeID_t node_id,
                                             NodeID_t parent_id,
                                             const QString & name,
                                             QDeviceConnection * device) override;

    virtual NodeControllerForm * createForm(QWidget * parent) override;

    virtual NodeType_t type() const override;

protected:
    virtual bool eventSetup(const ControlPacket &) override;

    virtual bool eventData(const ControlPacket & packet) override;
};

#endif // BOOLPROPERTY_H
