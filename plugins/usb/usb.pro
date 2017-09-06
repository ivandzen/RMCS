include( $$PWD/../../rmcs_config.pri )
include( $$PWD/../../core/core.pri )
include( $$PWD/../../qcore/qcore.pri )

TEMPLATE = lib

TARGET = rmcs_usb

QT += core gui widgets

CONFIG += c++11 plugin

DESTDIR = $$BUILD_PREFIX/share/rmcs/backends

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

win32 {
    INCLUDEPATH += ../ C:\libusb-1.0.21\include\libusb-1.0
    LIBS += C:\libusb-1.0.21\MinGW32\static\libusb-1.0.a
    DEFINES += "CALLBACK_ATTRIB=__attribute__((__stdcall__))"
}


unix {
    INCLUDEPATH += ../ /usr/include/libusb-1.0
    LIBS += -lusb-1.0
    DEFINES += "CALLBACK_ATTRIB="
}

DISTFILES += \
    usb.pri
