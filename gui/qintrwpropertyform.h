#ifndef QINTRWPROPERTYFORM_H
#define QINTRWPROPERTYFORM_H

#include <QWidget>
#include <gui/qnodeform.h>

namespace Ui {
class QIntRWPropertyForm;
}

class QRWPropertyController;

class QIntRWPropertyForm : public QNodeForm
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
    void valueChanged(const QVariant & value);
    void on_pushButton_clicked();
    void on_okButton_clicked();
};

#endif // QINTRWPROPERTYFORM_H
