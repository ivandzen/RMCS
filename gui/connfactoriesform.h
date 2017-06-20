#ifndef CONNFACTORIESFORM_H
#define CONNFACTORIESFORM_H

#include <QWidget>
#include <qcore/controller/qdevicecontrollerfactory.h>

namespace Ui {
class ConnFactoriesForm;
}

class ConnFactoriesForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConnFactoriesForm(QWidget *parent = 0);

    bool registerFactory(QDeviceControllerFactory * factory);

    ~ConnFactoriesForm();

private slots:
    void on_factoryCombo_activated(int index);

    void on_refreshButton_clicked();

    void on_openButton_clicked();

private:
    void updateDeviceList();

    Ui::ConnFactoriesForm * ui;
    QList<QDeviceControllerFactory*> _factories;
    QDeviceControllerFactory * _currentFactory;
};

#endif // CONNFACTORIESFORM_H
