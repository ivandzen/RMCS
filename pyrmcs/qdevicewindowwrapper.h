#ifndef QDEVICEWINDOWWRAPPER_H
#define QDEVICEWINDOWWRAPPER_H

#include <QObject>
#include <gui/qdevicewindow.h>

class QDeviceWindowWrapper :
        public QObject
{
    Q_OBJECT
public slots:
    QDeviceWindow * new_QDeviceWindow(QDeviceController * device,
                                      QWidget * parent = nullptr) {
        return new QDeviceWindow(device, parent);
    }

    void show(QDeviceWindow * wnd) {
        wnd->show();
    }

    void setVisible(QDeviceWindow * wnd, bool visible) {
        wnd->setVisible(visible);
    }
};

#endif // QDEVICEWINDOWWRAPPER_H
