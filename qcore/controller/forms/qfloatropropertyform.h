#ifndef QFLOATROPROPERTYFORM_H
#define QFLOATROPROPERTYFORM_H

#include <QWidget>
#include <qcore/controller/forms/nodecontrollerform.h>

namespace Ui {
class QFloatROPropertyForm;
}

class QROPropertyController;

class QFloatROPropertyForm : public NodeControllerForm
{
    Q_OBJECT

public:
    explicit QFloatROPropertyForm(QROPropertyController * prop,
                                  QWidget *parent = 0);
    ~QFloatROPropertyForm();

    QROPropertyController * node() const;

private:
    Ui::QFloatROPropertyForm *ui;

private slots:
    void dataUpdated();
    void on_updateButton_clicked();
};

#endif // QFLOATROPROPERTYFORM_H
