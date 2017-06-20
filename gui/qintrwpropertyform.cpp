#include "qintrwpropertyform.h"
#include "ui_qintrwpropertyform.h"
#include <qcore/controller/qrwpropertycontroller.h>

QIntRWPropertyForm::QIntRWPropertyForm(QRWPropertyController * prop,
                                       QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QIntRWPropertyForm)
{
    ui->setupUi(this);
    ui->spinBox->setValue(prop->get().toInt());
    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QIntRWPropertyForm::~QIntRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QIntRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(NodeControllerForm::node());
}

void QIntRWPropertyForm::valueChanged(const QVariant & value)
{
    bool ok = false;
    int ivalue = value.toInt(&ok);
    if(ok)
        ui->spinBox->setValue(ivalue);
}

void QIntRWPropertyForm::on_pushButton_clicked()
{
    node()->update();
}

void QIntRWPropertyForm::on_okButton_clicked()
{
    node()->set(ui->spinBox->value());
}
