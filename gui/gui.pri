QT += 3dcore 3drender 3dinput 3dlogic 3dextras 3danimation printsupport

LIBS += -L$$BUILD_PREFIX/lib/ -lrmcs_gui

DEPENDPATH += $$PWD

INCLUDEPATH += $$PWD/../
