#include "qfloatrwpropertyform.h"
#include "ui_qfloatrwpropertyform.h"
#include <qcore/controller/qrwpropertycontroller.h>

QFloatRWPropertyForm::QFloatRWPropertyForm(QRWPropertyController * prop,
                                       QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QFloatRWPropertyForm)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setValue(prop->get().toFloat());
    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QFloatRWPropertyForm::~QFloatRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QFloatRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(NodeControllerForm::node());
}

void QFloatRWPropertyForm::valueChanged(const QVariant & value)
{
    bool ok = false;
    float fvalue = value.toFloat(&ok);
    if(ok)
        ui->doubleSpinBox->setValue(fvalue);
}

void QFloatRWPropertyForm::on_pushButton_clicked()
{
    node()->update();
}

void QFloatRWPropertyForm::on_okButton_clicked()
{
    node()->set(ui->doubleSpinBox->value());
}
