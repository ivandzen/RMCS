#ifndef QOPARAMFORM_H
#define QOPARAMFORM_H

#include <QWidget>
#include <gui/qnodeform.h>

namespace Ui {
class QOParamForm;
}

class QOParamController;

class QOParamForm :
        public QNodeForm
{
    Q_OBJECT

public:
    explicit QOParamForm(QOParamController * node,
                         QWidget *parent = 0);
    ~QOParamForm();

    QOParamController * node() const;

private:
    //void updateWidget();

    Ui::QOParamForm *ui;

    /*
private slots:
    void onBufferFull();
    void on_resetButton_clicked();
    void on_pauseButton_clicked(bool checked);
    void on_bufSizeOkButton_clicked();
    */
};

#endif // QOPARAMFORM_H
