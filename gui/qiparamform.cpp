#include "qiparamform.h"
#include "ui_qiparamform.h"
#include <qcore/controller/qiparamcontroller.h>
#include <core/common/defaultnodes.h>
#include <limits>

QIParamForm::QIParamForm(QIParamController * param,
                         QWidget *parent) :
    NodeControllerForm(param, parent),
    ui(new Ui::QIParamForm)
{
    ui->setupUi(this);
    connect(this, SIGNAL(setValue(QVariant)),
            param, SLOT(setValue(QVariant)));
    switch(param->type())
    {
    case NODE_TYPE_INT8_IPARAM :
    {
        ui->doubleSpinBox->setVisible(false);
        ui->spinBox->setMinimum(std::numeric_limits<int8_t>::min());
        ui->spinBox->setMaximum(std::numeric_limits<int8_t>::max());
        break;
    }

    case NODE_TYPE_INT16_IPARAM :
    {
        ui->doubleSpinBox->setVisible(false);
        ui->spinBox->setMinimum(std::numeric_limits<int16_t>::min());
        ui->spinBox->setMaximum(std::numeric_limits<int16_t>::max());
        break;
    }

    case NODE_TYPE_INT32_IPARAM :
    {
        ui->doubleSpinBox->setVisible(false);
        ui->spinBox->setMinimum(std::numeric_limits<int32_t>::min());
        ui->spinBox->setMaximum(std::numeric_limits<int32_t>::max());
        break;
    }

    case NODE_TYPE_FLOAT32_IPARAM :
    {
        ui->spinBox->setVisible(false);
        ui->doubleSpinBox->setMinimum(-std::numeric_limits<float>::max());
        ui->doubleSpinBox->setMaximum(std::numeric_limits<float>::max());
        break;
    }

    case NODE_TYPE_FLOAT64_IPARAM :
    {
        ui->spinBox->setVisible(false);
        ui->doubleSpinBox->setMinimum(-std::numeric_limits<double>::max());
        ui->doubleSpinBox->setMaximum(std::numeric_limits<double>::max());
        break;
    }
    }
}

QIParamForm::~QIParamForm()
{
    delete ui;
}

QIParamController * QIParamForm::node() const
{
    return static_cast<QIParamController*>(NodeControllerForm::node());
}

void QIParamForm::on_spinBox_valueChanged(int arg1)
{
    emit setValue(arg1);
}

void QIParamForm::on_doubleSpinBox_valueChanged(double arg1)
{
    emit setValue(arg1);
}
