#include "qintrwpropertyform.h"
#include "ui_qintrwpropertyform.h"
#include <qcore/controller/properties/qrwpropertycontroller.h>

QIntRWPropertyForm::QIntRWPropertyForm(QRWPropertyController * prop,
                                       QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QIntRWPropertyForm)
{
    ui->setupUi(this);
    connect(prop, SIGNAL(updated()), this, SLOT(dataUpdated()));
}

QIntRWPropertyForm::~QIntRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QIntRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(NodeControllerForm::node());
}

void QIntRWPropertyForm::dataUpdated()
{
    bool ok = false;
    int value = node()->get().toInt(&ok);
    if(ok)
        ui->spinBox->setValue(value);
}

void QIntRWPropertyForm::on_pushButton_clicked()
{
    node()->update();
}

void QIntRWPropertyForm::on_okButton_clicked()
{
    node()->set(ui->spinBox->value());
}
