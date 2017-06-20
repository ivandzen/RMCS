#include "connfactoriesform.h"
#include "ui_connfactoriesform.h"

ConnFactoriesForm::ConnFactoriesForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnFactoriesForm),
    _currentFactory(nullptr)
{
    ui->setupUi(this);
}

bool ConnFactoriesForm::registerFactory(QDeviceControllerFactory * factory)
{
    if(_factories.contains(factory))
        return false;

    ui->factoryCombo->addItem(factory->factoryName(),
                              _factories.size());
    _factories.push_back(factory);
    return true;
}

ConnFactoriesForm::~ConnFactoriesForm()
{
    delete ui;
}

void ConnFactoriesForm::on_factoryCombo_activated(int index)
{
    bool ok = false;
    int idx = ui->factoryCombo->itemData(index).toInt(&ok);
    if(!ok)
        return;
    _currentFactory = _factories[idx];
    updateDeviceList();
}

void ConnFactoriesForm::updateDeviceList()
{
    _currentFactory->refresh();
    QStringList names = _currentFactory->devicesNames();
    ui->deviceList->clear();
    for(QString & name : names)
        ui->deviceList->addItem(name);
}

void ConnFactoriesForm::on_refreshButton_clicked()
{
    updateDeviceList();
}

void ConnFactoriesForm::on_openButton_clicked()
{

}
