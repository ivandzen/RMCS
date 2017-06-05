include( $$PWD/../rmcs_config.pri )
include( $$PWD/../core/core.pri )
include( $$PWD/../qcore/qcore.pri )

TEMPLATE = lib

TARGET = rmcs_usb

QT += core gui widgets

CONFIG += c++11 plugin

DESTDIR = $$BUILD_PREFIX/share/rmcs/plug-ins

HEADERS += \
    host/qusbconnection.h \
    host/qusbdevicefactory.h \
    host/qusbistreamcontroller.h \
    host/qusbostreamcontroller.h

SOURCES += \
    host/qusbconnection.cpp \
    host/qusbdevicefactory.cpp \
    host/qusbistreamcontroller.cpp \
    host/qusbostreamcontroller.cpp

INCLUDEPATH += ../ /usr/include/libusb-1.0

LIBS += -lusb-1.0

DISTFILES += \
    usb.pri
