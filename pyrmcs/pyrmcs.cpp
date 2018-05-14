#include "pyrmcs.h"
#include "qrmcswrapper.h"
#include "qdevicecontrollerfactorywrapper.h"
#include "qdevicecontrollerwrapper.h"
#include "qnodecontrollerwrapper.h"
#include "qnodecontrollerfactorywrapper.h"
#include "qistreamcontrollerwrapper.h"
#include "qostreamcontrollerwrapper.h"
#include "qropropertycontrollerwrapper.h"
#include "qrwpropertycontrollerwrapper.h"
#include "qiparamcontrollerwrapper.h"
#include "qoparamcontrollerwrapper.h"
#include "qdevicewindowwrapper.h"
#include "graphformwrapper.h"
#include "PythonQt.h"
#include <QDebug>

void PythonRmcs::init()
{
    PythonQt::self()->registerCPPClass("QRmcs",
                                       "", "Rmcs",
                                       PythonQtCreateObject<QRmcsWrapper>);

    PythonQt::self()->registerCPPClass("QDeviceControllerFactory",
                                       "PythonQt.QtCore.QObject", "Rmcs",
                                       PythonQtCreateObject<QDeviceControllerFactoryWrapper>);
    PythonQt::self()->registerCPPClass("QDeviceController",
                                       "PythonQt.QtCore.QObject", "Rmcs",
                                       PythonQtCreateObject<QDeviceControllerWrapper>);
    PythonQt::self()->registerCPPClass("QNodeController",
                                       "PythonQt.QtCore.QObject", "Rmcs",
                                       PythonQtCreateObject<QNodeControllerWrapper>);
    PythonQt::self()->registerCPPClass("QNodeControllerFactory",
                                       "PythonQt.QtCore.QObject", "Rmcs",
                                       PythonQtCreateObject<QNodeControllerFactoryWrapper>);
    PythonQt::self()->registerCPPClass("QIStreamController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QIStreamControllerWrapper>);
    PythonQt::self()->registerCPPClass("QOStreamController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QOStreamControllerWrapper>);
    PythonQt::self()->registerCPPClass("QROPropertyController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QROPropertyControllerWrapper>);
    PythonQt::self()->registerCPPClass("QRWPropertyController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QRWPropertyControllerWrapper>);
    PythonQt::self()->registerCPPClass("QIParamController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QIParamControllerWrapper>);
    PythonQt::self()->registerCPPClass("QOParamController",
                                       "Rmcs.QNodeController", "Rmcs",
                                       PythonQtCreateObject<QOParamControllerWrapper>);
    PythonQt::self()->registerCPPClass("QDeviceWindow",
                                       "", "Rmcs",
                                       PythonQtCreateObject<QDeviceWindowWrapper>);
    PythonQt::self()->registerCPPClass("GraphForm",
                                       "", "Rmcs",
                                       PythonQtCreateObject<GraphFormWrapper>);
}
