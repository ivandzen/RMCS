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
    controller/qdevicecontrollerfactory.h \
    controller/qdeviceconnection.h \
    controller/qnodecontrollerfactory.h \
    controller/qoparamcontroller.h \
    controller/qiparamcontroller.h \
    controller/qropropertycontroller.h \
    controller/qrwpropertycontroller.h \
    controller/qdefaultcontroller.h \
    controller/qrmcs.h \
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
    controller/qdeviceconnection.cpp \
    controller/qnodecontrollerfactory.cpp \
    controller/qoparamcontroller.cpp \
    controller/qiparamcontroller.cpp \
    controller/qropropertycontroller.cpp \
    controller/qrwpropertycontroller.cpp \
    controller/qdefaultcontroller.cpp \
    controller/qrmcs.cpp \
    #device/tcpistreamnode.cpp \
    #controller/qtcpistreamcontroller.cpp

INCLUDEPATH += ../


