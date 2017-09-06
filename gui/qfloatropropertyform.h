#ifndef QFLOATROPROPERTYFORM_H
#define QFLOATROPROPERTYFORM_H

#include <QWidget>
#include <gui/qnodeform.h>

namespace Ui {
class QFloatROPropertyForm;
}

class QROPropertyController;

class QFloatROPropertyForm : public QNodeForm
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
    void valueChanged(const QVariant & value);
    void on_updateButton_clicked();
};

#endif // QFLOATROPROPERTYFORM_H
