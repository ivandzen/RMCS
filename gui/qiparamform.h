#ifndef QIPARAMFORM_H
#define QIPARAMFORM_H

#include <QWidget>
#include <QVariant>
#include <gui/nodecontrollerform.h>

namespace Ui {
class QIParamForm;
}

class QIParamController;

class QIParamForm :
        public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QIParamForm(QIParamController * param,
                         QWidget *parent = 0);
    ~QIParamForm();

    QIParamController * node() const;

private:
    Ui::QIParamForm *ui;

signals :
    void setValue(QVariant);
private slots:
    void on_spinBox_valueChanged(int arg1);
    void on_doubleSpinBox_valueChanged(double arg1);
};

#endif // QIPARAMFORM_H
