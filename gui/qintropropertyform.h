#ifndef QINTROPROPERTYFORM_H
#define QINTROPROPERTYFORM_H

#include <QWidget>
#include <gui/nodecontrollerform.h>

namespace Ui {
class QIntROPropertyForm;
}

class QROPropertyController;

class QIntROPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QIntROPropertyForm(QROPropertyController * prop,
                                QWidget *parent = 0);
    ~QIntROPropertyForm();

    QROPropertyController * node() const;

private:
    Ui::QIntROPropertyForm *ui;

private slots:
    void valueChanged(const QVariant & value);
    void on_updateButton_clicked();
};

#endif // QINTROPROPERTYFORM_H
