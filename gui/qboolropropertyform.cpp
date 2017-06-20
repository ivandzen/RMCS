#include "qboolropropertyform.h"
#include "ui_qboolropropertyform.h"
#include <qcore/controller/qropropertycontroller.h>

QBoolROPropertyForm::QBoolROPropertyForm(QROPropertyController * prop,
                                         QWidget *parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QBoolROPropertyForm)
{
    ui->setupUi(this);
    if(prop->get().toBool())
        ui->label->setText("True");
    else
        ui->label->setText("False");

    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QBoolROPropertyForm::~QBoolROPropertyForm()
{
    delete ui;
}

QROPropertyController * QBoolROPropertyForm::node() const
{
    return static_cast<QROPropertyController*>(NodeControllerForm::node());
}

void QBoolROPropertyForm::valueChanged(const QVariant &value)
{
    if(value.toBool())
        ui->label->setText("True");
    else
        ui->label->setText("False");
}

void QBoolROPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
