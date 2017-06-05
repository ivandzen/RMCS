#include "ostreamform.h"
#include "ui_ostreamform.h"

OStreamForm::OStreamForm(QOStreamController * stream,
                         QWidget * parent) :
    NodeControllerForm (stream, parent),
    ui(new Ui::OStreamForm)
{
    ui->setupUi(this);
    connect(stream, SIGNAL(streamToggled(bool)),
            this, SLOT(streamToggled(bool)));
}

OStreamForm::~OStreamForm()
{
    delete ui;
}

void OStreamForm::streamToggled(bool enabled)
{
    ui->streamEnabledBox->setChecked(enabled);
}

void OStreamForm::on_streamEnabledBox_clicked(bool checked)
{
    node()->setStreamEnabled(checked);
}
