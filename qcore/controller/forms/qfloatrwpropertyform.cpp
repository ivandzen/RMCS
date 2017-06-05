#include "qfloatrwpropertyform.h"
#include "ui_qfloatrwpropertyform.h"
#include <qcore/controller/properties/qrwpropertycontroller.h>

QFloatRWPropertyForm::QFloatRWPropertyForm(QRWPropertyController * prop,
                                       QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QFloatRWPropertyForm)
{
    ui->setupUi(this);
    connect(prop, SIGNAL(updated()), this, SLOT(dataUpdated()));
}

QFloatRWPropertyForm::~QFloatRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QFloatRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(NodeControllerForm::node());
}

void QFloatRWPropertyForm::dataUpdated()
{
    bool ok = false;
    float value = node()->get().toFloat(&ok);
    if(ok)
        ui->doubleSpinBox->setValue(value);
}

void QFloatRWPropertyForm::on_pushButton_clicked()
{
    node()->update();
}

void QFloatRWPropertyForm::on_okButton_clicked()
{
    node()->set(ui->doubleSpinBox->value());
}
