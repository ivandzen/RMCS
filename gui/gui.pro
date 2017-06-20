include( $$PWD/../rmcs_config.pri )
include( $$PWD/../core/core.pri )
include( $$PWD/../qcore/qcore.pri )

TEMPLATE = lib

QT += core network widgets qml

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
    qoparamform.ui

HEADERS += \
    connfactoriesform.h \
    nodecontrollerform.h \
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
    qnodeformfactory.h

SOURCES += \
    connfactoriesform.cpp \
    nodecontrollerform.cpp \
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
    qnodeformfactory.cpp

