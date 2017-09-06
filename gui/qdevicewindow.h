#ifndef QDEVICEWINDOW_H
#define QDEVICEWINDOW_H

#include <QMainWindow>
#include <qcore/controller/qdevicecontroller.h>
#include <gui/qdevicemodel.h>
#include <gui/qnodeformfactory.h>

namespace Ui {
class QDeviceWindow;
}

class QDeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QDeviceWindow(QDeviceController *device,
                           QWidget * parent = 0);
    ~QDeviceWindow();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

private:
    void clearForms();

    void createForms(QNodeController * node);

    Ui::QDeviceWindow *ui;
    QDeviceModel _model;
    QVector<QNodeForm*> _forms;
    QNodeFormFactory _factory;
};

#endif // QDEVICEWINDOW_H
