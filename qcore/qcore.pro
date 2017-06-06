include( $$PWD/../rmcs_config.pri )
include( $$PWD/../core/core.pri )

QT += core network widgets qml

TEMPLATE = lib

CONFIG += c++11

TARGET = rmcs_qcore

DESTDIR = $$BUILD_PREFIX/lib

HEADERS += \
    controller/qdevicecontroller.h \
    controller/qnodecontroller.h \
    #controller/tcpbackend.h \
    #device/tcpoutputstreamnode.h \
    controller/qistreamcontroller.h \
    controller/qostreamcontroller.h \
    controller/qistreamchannelcontroller.h \
    controller/qostreamchannelcontroller.h \
    controller/forms/nodecontrollerform.h \
    controller/forms/ostreamform.h \
    controller/forms/istreamform.h \
    controller/forms/connfactoriesform.h \
    controller/qdevicecontrollerfactory.h \
    controller/supervisor.h \
    controller/qdeviceconnection.h \
    controller/qnodecontrollerfactory.h \
    controller/forms/qboolropropertyform.h \
    controller/forms/qintropropertyform.h \
    controller/forms/qfloatropropertyform.h \
    controller/forms/qboolrwpropertyform.h \
    controller/forms/qintrwpropertyform.h \
    controller/forms/qfloatrwpropertyform.h \
    controller/qoparamcontroller.h \
    controller/qiparamcontroller.h \
    controller/qropropertycontroller.h \
    controller/qrwpropertycontroller.h
    #device/tcpistreamnode.h \
    #controller/qtcpistreamcontroller.h

SOURCES += \
    controller/qdevicecontroller.cpp \
    controller/qnodecontroller.cpp \
    #controller/tcpbackend.cpp \
    #device/tcpoutputstreamnode.cpp \
    controller/qistreamcontroller.cpp \
    controller/qostreamcontroller.cpp \
    controller/qistreamchannelcontroller.cpp \
    controller/qostreamchannelcontroller.cpp \
    controller/forms/nodecontrollerform.cpp \
    controller/forms/ostreamform.cpp \
    controller/forms/istreamform.cpp \
    controller/forms/connfactoriesform.cpp \
    controller/supervisor.cpp \
    controller/qdeviceconnection.cpp \
    controller/qnodecontrollerfactory.cpp \
    controller/forms/qboolropropertyform.cpp \
    controller/forms/qintropropertyform.cpp \
    controller/forms/qfloatropropertyform.cpp \
    controller/forms/qboolrwpropertyform.cpp \
    controller/forms/qintrwpropertyform.cpp \
    controller/forms/qfloatrwpropertyform.cpp \
    controller/qoparamcontroller.cpp \
    controller/qiparamcontroller.cpp \
    controller/qropropertycontroller.cpp \
    controller/qrwpropertycontroller.cpp
    #device/tcpistreamnode.cpp \
    #controller/qtcpistreamcontroller.cpp

INCLUDEPATH += ../
FORMS += \
    controller/forms/ostreamform.ui \
    controller/forms/istreamform.ui \
    controller/forms/connfactoriesform.ui \
    controller/forms/qboolropropertyform.ui \
    controller/forms/qintropropertyform.ui \
    controller/forms/qfloatropropertyform.ui \
    controller/forms/qboolrwpropertyform.ui \
    controller/forms/qintrwpropertyform.ui \
    controller/forms/qfloatrwpropertyform.ui

