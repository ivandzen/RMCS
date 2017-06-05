#ifndef QINTRWPROPERTYFORM_H
#define QINTRWPROPERTYFORM_H

#include <QWidget>
#include <qcore/controller/forms/nodecontrollerform.h>

namespace Ui {
class QIntRWPropertyForm;
}

class QRWPropertyController;

class QIntRWPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QIntRWPropertyForm(QRWPropertyController * prop,
                                QWidget *parent = 0);
    ~QIntRWPropertyForm();

    QRWPropertyController * node() const;

private:
    Ui::QIntRWPropertyForm *ui;

private slots:
    void dataUpdated();
    void on_pushButton_clicked();
    void on_okButton_clicked();
};

#endif // QINTRWPROPERTYFORM_H
