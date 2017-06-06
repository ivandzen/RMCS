#include "qboolrwpropertyform.h"
#include "ui_qboolrwpropertyform.h"
#include <qcore/controller/qrwpropertycontroller.h>

QBoolRWPropertyForm::QBoolRWPropertyForm(QRWPropertyController * prop,
                                         QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QBoolRWPropertyForm)
{
    ui->setupUi(this);
    connect(prop, SIGNAL(updated()), this, SLOT(dataUpdated()));
}

QBoolRWPropertyForm::~QBoolRWPropertyForm()
{
    delete ui;
}

QRWPropertyController *QBoolRWPropertyForm::node() const
{
    return static_cast<QRWPropertyController*>(NodeControllerForm::node());
}

void QBoolRWPropertyForm::dataUpdated()
{
    ui->checkBox->setChecked(node()->get().toBool());
}

void QBoolRWPropertyForm::on_checkBox_clicked(bool checked)
{
    node()->set(QVariant(checked));
}

void QBoolRWPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
