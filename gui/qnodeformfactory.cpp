#include "qnodeformfactory.h"
#include <qcore/controller/qnodecontroller.h>
#include <core/common/defaultnodes.h>

#include <qcore/controller/qropropertycontroller.h>
#include <qcore/controller/qrwpropertycontroller.h>
#include <qcore/controller/qiparamcontroller.h>
#include <qcore/controller/qoparamcontroller.h>

#include <gui/qoparamform.h>
#include <gui/qiparamform.h>

#include <gui/qboolropropertyform.h>
#include <gui/qboolrwpropertyform.h>
#include <gui/qintropropertyform.h>
#include <gui/qintrwpropertyform.h>
#include <gui/qfloatropropertyform.h>
#include <gui/qfloatrwpropertyform.h>

NodeControllerForm * QNodeFormFactory::createForm(QNodeController * controller,
                                                  QWidget * parent)
{
    switch(controller->type())
    {
    case NODE_TYPE_DEFAULT : break;

    ////////////////////////////////////////////////////////////////////////////////////

    case NODE_TYPE_INT8_IPARAM :
    case NODE_TYPE_INT16_IPARAM :
    case NODE_TYPE_INT32_IPARAM :
    case NODE_TYPE_FLOAT32_IPARAM :
    case NODE_TYPE_FLOAT64_IPARAM :
    {
        QIParamController * cntrl = qobject_cast<QIParamController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QIParamForm(cntrl, parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////

    case NODE_TYPE_INT8_OPARAM :
    case NODE_TYPE_INT16_OPARAM :
    case NODE_TYPE_INT32_OPARAM :
    case NODE_TYPE_FLOAT32_OPARAM :
    case NODE_TYPE_FLOAT64_OPARAM :
    {
        QOParamController * cntrl = qobject_cast<QOParamController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QOParamForm(cntrl, parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////

    case NODE_TYPE_ENUM_PROP_RDONLY : break; //!@ todo

    case NODE_TYPE_BOOL_PROP_RDONLY :
    {
        QROPropertyController * cntrl = qobject_cast<QROPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QBoolROPropertyForm(cntrl, parent);
    }

    case NODE_TYPE_INT_PROP_RDONLY :
    {
        QROPropertyController * cntrl = qobject_cast<QROPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QIntROPropertyForm(cntrl, parent);
    }

    case NODE_TYPE_FLOAT_PROP_RDONLY :
    {
        QROPropertyController * cntrl = qobject_cast<QROPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QFloatROPropertyForm(cntrl, parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////

    case NODE_TYPE_ENUM_PROP : break; //!@ todo

    case NODE_TYPE_BOOL_PROP :
    {
        QRWPropertyController * cntrl = qobject_cast<QRWPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QBoolRWPropertyForm(cntrl, parent);
    }

    case NODE_TYPE_INT_PROP :
    {
        QRWPropertyController * cntrl = qobject_cast<QRWPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QIntRWPropertyForm(cntrl, parent);
    }

    case NODE_TYPE_FLOAT_PROP :
    {
        QRWPropertyController * cntrl = qobject_cast<QRWPropertyController*>(controller);
        if(cntrl == nullptr)
            return nullptr;
        return new QFloatRWPropertyForm(cntrl, parent);
    }

    default : break;
    }

    return nullptr;
}
