#include "qoparamform.h"
#include "ui_qoparamform.h"
#include <qcore/controller/qoparamcontroller.h>

QOParamForm::QOParamForm(QOParamController * node,
                         QWidget *parent) :
    NodeControllerForm(node, parent),
    ui(new Ui::QOParamForm)
{
    ui->setupUi(this);
    connect(node, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged(QVariant)));
}

QOParamForm::~QOParamForm()
{
    delete ui;
}

QOParamController *QOParamForm::node() const
{
    return static_cast<QOParamController*>(NodeControllerForm::node());
}

void QOParamForm::valueChanged(const QVariant &value)
{
    ui->label->setText(value.toString());
}
