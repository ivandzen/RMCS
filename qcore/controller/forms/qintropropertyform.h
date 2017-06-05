#ifndef QINTROPROPERTYFORM_H
#define QINTROPROPERTYFORM_H

#include <QWidget>
#include <qcore/controller/forms/nodecontrollerform.h>

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
    void dataUpdated();
    void on_updateButton_clicked();
};

#endif // QINTROPROPERTYFORM_H
