#include "qfloatropropertyform.h"
#include "ui_qfloatropropertyform.h"
#include <qcore/controller/qropropertycontroller.h>

QFloatROPropertyForm::QFloatROPropertyForm(QROPropertyController *prop,
                                           QWidget *parent) :
    QNodeForm (prop, parent),
    ui(new Ui::QFloatROPropertyForm)
{
    ui->setupUi(this);
    ui->label->setText(prop->get().toString());
    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QFloatROPropertyForm::~QFloatROPropertyForm()
{
    delete ui;
}

QROPropertyController *QFloatROPropertyForm::node() const
{
    return static_cast<QROPropertyController*>(QNodeForm::node());
}

void QFloatROPropertyForm::valueChanged(const QVariant & value)
{
    ui->label->setText(value.toString());
}

void QFloatROPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
