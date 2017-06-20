#ifndef QBOOLRWPROPERTYFORM_H
#define QBOOLRWPROPERTYFORM_H

#include <QWidget>
#include <gui/nodecontrollerform.h>

namespace Ui {
class QBoolRWPropertyForm;
}

class QRWPropertyController;

class QBoolRWPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QBoolRWPropertyForm(QRWPropertyController * prop,
                                 QWidget *parent = 0);
    ~QBoolRWPropertyForm();

    QRWPropertyController * node() const;

private:
    Ui::QBoolRWPropertyForm *ui;

private slots:
    void valueChanged(const QVariant & value);
    void on_checkBox_clicked(bool checked);
    void on_updateButton_clicked();
};

#endif // QBOOLRWPROPERTYFORM_H
