include( $$PWD/../rmcs_config.pri )
include( $$PWD/../core/core.pri )
include( $$PWD/../qcore/qcore.pri )

TEMPLATE = lib

QT += core network widgets qml 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation

CONFIG += c++11

TARGET = rmcs_gui

DESTDIR = $$BUILD_PREFIX/lib

FORMS += \
    connfactoriesform.ui \
    qboolropropertyform.ui \
    qboolrwpropertyform.ui \
    qdevicewindow.ui \
    qfloatropropertyform.ui \
    qfloatrwpropertyform.ui \
    qintropropertyform.ui \
    qintrwpropertyform.ui \
    qiparamform.ui \
    qoparamform.ui \
    graphform.ui \
    sensormodelform.ui

HEADERS += \
    connfactoriesform.h \
    qboolropropertyform.h \
    qboolrwpropertyform.h \
    qdevicemodel.h \
    qdevicewindow.h \
    qfloatropropertyform.h \
    qfloatrwpropertyform.h \
    qintropropertyform.h \
    qintrwpropertyform.h \
    qiparamform.h \
    qoparamform.h \
    qnodeformfactory.h \
    qnodeform.h \
    qcustomplot.h \
    graphform.h \
    sensormodelform.h

SOURCES += \
    connfactoriesform.cpp \
    qboolropropertyform.cpp \
    qboolrwpropertyform.cpp \
    qdevicemodel.cpp \
    qdevicewindow.cpp \
    qfloatropropertyform.cpp \
    qfloatrwpropertyform.cpp \
    qintropropertyform.cpp \
    qintrwpropertyform.cpp \
    qiparamform.cpp \
    qoparamform.cpp \
    qnodeformfactory.cpp \
    qnodeform.cpp \
    qcustomplot.cpp \
    graphform.cpp \
    sensormodelform.cpp

