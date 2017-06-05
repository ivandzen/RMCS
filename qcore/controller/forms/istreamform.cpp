#include "istreamform.h"
#include "ui_istreamform.h"

IStreamForm::IStreamForm(QIStreamController * stream,
                         QWidget *parent) :
    NodeControllerForm (stream, parent),
    ui(new Ui::IStreamForm)
{
    ui->setupUi(this);
    connect(stream, SIGNAL(streamToggled(bool)),
            this, SLOT(streamToggled(bool)));
}

IStreamForm::~IStreamForm()
{
    delete ui;
}

void IStreamForm::streamToggled(bool enabled)
{
    ui->streamEnabledBox->setChecked(enabled);
}


void IStreamForm::on_streamEnabledBox_clicked(bool checked)
{
    node()->setStreamEnabled(checked);
}
