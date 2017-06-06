#include "qintropropertyform.h"
#include "ui_qintropropertyform.h"
#include <qcore/controller/qropropertycontroller.h>

QIntROPropertyForm::QIntROPropertyForm(QROPropertyController * prop,
                                       QWidget * parent) :
    NodeControllerForm (prop, parent),
    ui(new Ui::QIntROPropertyForm)
{
    ui->setupUi(this);
    connect(prop, SIGNAL(updated()), this, SLOT(dataUpdated()));
}

QIntROPropertyForm::~QIntROPropertyForm()
{
    delete ui;
}

QROPropertyController *QIntROPropertyForm::node() const
{
    return static_cast<QROPropertyController*>(NodeControllerForm::node());
}

void QIntROPropertyForm::dataUpdated()
{
    ui->label->setText(node()->get().toString());
}

void QIntROPropertyForm::on_updateButton_clicked()
{
    node()->update();
}
