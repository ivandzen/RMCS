#include "qboolrwpropertyform.h"
#include "ui_qboolrwpropertyform.h"
#include <qcore/controller/qrwpropertycontroller.h>

QBoolRWPropertyForm::QBoolRWPropertyForm(QRWPropertyController * prop,
                                         QWidget * parent) :
    QNodeForm (prop, parent),
    ui(new Ui::QBoolRWPropertyForm)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(prop->get().toBool());
    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QBoolRWPropertyForm::~QBoolRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QBoolRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(QNodeForm::node());
}

void QBoolRWPropertyForm::valueChanged(const QVariant & value)
{
    ui->checkBox->setChecked(value.toBool());
}

void QBoolRWPropertyForm::on_checkBox_clicked(bool checked)
{
    node()->set(QVariant(checked));
}

void QBoolRWPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
