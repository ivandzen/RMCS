#ifndef QFLOATRWPROPERTYFORM_H
#define QFLOATRWPROPERTYFORM_H

#include <QWidget>
#include <qcore/controller/forms/nodecontrollerform.h>

namespace Ui {
class QFloatRWPropertyForm;
}

class QRWPropertyController;

class QFloatRWPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QFloatRWPropertyForm(QRWPropertyController * prop,
                                  QWidget *parent = 0);
    ~QFloatRWPropertyForm();

    QRWPropertyController * node() const;

private:
    Ui::QFloatRWPropertyForm *ui;

private slots:
    void dataUpdated();
    void on_pushButton_clicked();
    void on_okButton_clicked();
};

#endif // QFLOATRWPROPERTYFORM_H
