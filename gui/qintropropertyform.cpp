#include "qintropropertyform.h"
#include "ui_qintropropertyform.h"
#include <qcore/controller/qropropertycontroller.h>

QIntROPropertyForm::QIntROPropertyForm(QROPropertyController * prop,
                                       QWidget * parent) :
    QNodeForm (prop, parent),
    ui(new Ui::QIntROPropertyForm)
{
    ui->setupUi(this);
    ui->label->setText(prop->get().toString());
    connect(prop, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QIntROPropertyForm::~QIntROPropertyForm()
{
    delete ui;
}

QROPropertyController *QIntROPropertyForm::node() const
{
    return static_cast<QROPropertyController*>(QNodeForm::node());
}

void QIntROPropertyForm::valueChanged(const QVariant & value)
{
    ui->label->setText(value.toString());
}

void QIntROPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
