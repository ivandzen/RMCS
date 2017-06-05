include($$PWD/../rmcs_config.pri)

TEMPLATE = lib

TARGET = rmcs_core

CONFIG += staticlib c++11

DESTDIR = $$BUILD_PREFIX/lib

QT += gui core widgets network

HEADERS += \
    common/arrayref.h \
    common/control_protocol.h \
    common/inputstream.h \
    common/outputstream.h \
    controller/devicecontroller.h \
    controller/nodecontroller.h \
    device/device.h \
    device/node.h \
    device/property.h \
    common/types.h \
    device/nodes.h \
    device/istreamnode.h \
    device/ostreamnode.h \
    device/istreamchannel.h \
    device/ostreamchannel.h \
    device/iparam.h \
    device/oparam.h \
    controller/deviceconnection.h \
    common/tcp.h \
    common/defaultnodes.h

SOURCES += \
    common/inputstream.cpp \
    common/outputstream.cpp \
    controller/devicecontroller.cpp \
    controller/nodecontroller.cpp \
    device/device.cpp \
    device/node.cpp \
    device/istreamnode.cpp \
    device/ostreamnode.cpp \
    device/istreamchannel.cpp \
    device/ostreamchannel.cpp \
    controller/deviceconnection.cpp

INCLUDEPATH += "../"
