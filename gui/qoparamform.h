#ifndef QOPARAMFORM_H
#define QOPARAMFORM_H

#include <QWidget>
#include <gui/nodecontrollerform.h>

namespace Ui {
class QOParamForm;
}

class QOParamController;

class QOParamForm :
        public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QOParamForm(QOParamController * node,
                         QWidget *parent = 0);
    ~QOParamForm();

    QOParamController * node() const;

private:
    Ui::QOParamForm *ui;

private slots:
    void valueChanged(const QVariant & value);
};

#endif // QOPARAMFORM_H
