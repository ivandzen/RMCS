#ifndef QBOOLROPROPERTYFORM_H
#define QBOOLROPROPERTYFORM_H

#include <QWidget>
#include <qcore/controller/forms/nodecontrollerform.h>

namespace Ui {
class QBoolROPropertyForm;
}

class QROPropertyController;

class QBoolROPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QBoolROPropertyForm(QROPropertyController * prop,
                                 QWidget *parent = 0);
    ~QBoolROPropertyForm();

    QROPropertyController * node() const;

private:
    Ui::QBoolROPropertyForm *ui;

private slots:
    void dataUpdated();
    void on_updateButton_clicked();
};

#endif // QBOOLROPROPERTYFORM_H