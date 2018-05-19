
TEMPLATE = lib

contains(QT_MAJOR_VERSION, 5) {
  QT += widgets
}

PYTHONQT_PATH = $$PWD/../../3rd_party/PythonQt

include ( $$PYTHONQT_PATH/build/PythonQt.prf )
include ( $$PYTHONQT_PATH/build/PythonQt_QtAll.prf )
include( $$PWD/../rmcs_config.pri )
include( $$PWD/../core/core.pri )
include( $$PWD/../qcore/qcore.pri )

DESTDIR           = $$BUILD_PREFIX/lib

HEADERS += \
    qdevicecontrollerfactorywrapper.h \
    qdevicecontrollerwrapper.h \
    qiparamcontrollerwrapper.h \
    qistreamcontrollerwrapper.h \
    qnodecontrollerfactorywrapper.h \
    qnodecontrollerwrapper.h \
    qoparamcontrollerwrapper.h \
    qostreamcontrollerwrapper.h \
    qrmcswrapper.h \
    qropropertycontrollerwrapper.h \
    qrwpropertycontrollerwrapper.h \
    pyrmcs.h \
    qdevicewindowwrapper.h \
    graphformwrapper.h \
    qint16oparamcontrollerwrapper.h

SOURCES += \
    pyrmcs.cpp \
    qint16oparamcontrollerwrapper.cpp
