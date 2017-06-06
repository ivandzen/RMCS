#include "qfloatropropertyform.h"
#include "ui_qfloatropropertyform.h"
#include <qcore/controller/qropropertycontroller.h>

QFloatROPropertyForm::QFloatROPropertyForm(QROPropertyController *prop,
                                           QWidget *parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QFloatROPropertyForm)
{
    ui->setupUi(this);
    connect(prop, SIGNAL(updated()), this, SLOT(dataUpdated()));
}

QFloatROPropertyForm::~QFloatROPropertyForm()
{
    delete ui;
}

QROPropertyController *QFloatROPropertyForm::node() const
{
    return static_cast<QROPropertyController*>(NodeControllerForm::node());
}

void QFloatROPropertyForm::dataUpdated()
{
    ui->label->setText(node()->get().toString());
}

void QFloatROPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
