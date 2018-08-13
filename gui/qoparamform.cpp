#include "qoparamform.h"
#include "ui_qoparamform.h"
#include <qcore/controller/qoparamcontroller.h>
#include <QDebug>

QOParamForm::QOParamForm(QOParamController * node,
                         QWidget *parent) :
    QNodeForm(node, parent),
    ui(new Ui::QOParamForm)
{
    ui->setupUi(this);

    connect(node, SIGNAL(bufferFull()), this, SLOT(onBufferFull()));
    //updateWidget();
}

QOParamForm::~QOParamForm()
{
    delete ui;
}

QOParamController *QOParamForm::node() const
{
    return static_cast<QOParamController*>(QNodeForm::node());
}

/*
void QOParamForm::updateWidget()
{
    switch(node()->state())
    {
    case QOParamController::STATE_RUNNING :
        ui->stateLabel->setText("RUNNING");
        ui->pauseButton->setChecked(false);
        ui->pauseButton->setText("Pause");
        break;

    case QOParamController::STATE_STOPPED :
        ui->stateLabel->setText("STOPPED");
        ui->pauseButton->setChecked(true);
        ui->pauseButton->setText("Resume");
        break;

    case QOParamController::STATE_BUF_FULL :
        ui->stateLabel->setText("BUF FULL");
        ui->pauseButton->setChecked(true);
        ui->pauseButton->setText("Resume");
        break;
    }

    ui->bufSizeSpin->setValue(node()->bufferSize());
}

void QOParamForm::onBufferFull()
{
    updateWidget();
}

void QOParamForm::on_resetButton_clicked()
{
    node()->reset();
    updateWidget();
}

void QOParamForm::on_pauseButton_clicked(bool checked)
{
    if(checked)
        node()->pause();
    else
        node()->reset();
    updateWidget();
}

void QOParamForm::on_bufSizeOkButton_clicked()
{
    node()->setBufferSize(ui->bufSizeSpin->value());
    updateWidget();
}
*/
