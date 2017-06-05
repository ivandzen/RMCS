#ifndef OSTREAMFORM_H
#define OSTREAMFORM_H
#include <qcore/controller/forms/nodecontrollerform.h>
#include <qcore/controller/qostreamcontroller.h>

namespace Ui {
class OStreamForm;
}

class OStreamForm :
        public NodeControllerForm
{
    Q_OBJECT

public:
    explicit OStreamForm(QOStreamController * stream,
                         QWidget * parent = 0);
    ~OStreamForm();

    inline QOStreamController * node() const {
        return reinterpret_cast<QOStreamController*>(NodeControllerForm::node());
    }

private:
    Ui::OStreamForm *ui;

private slots:
    void streamToggled(bool enabled);
    void on_streamEnabledBox_clicked(bool checked);
};

#endif // OSTREAMFORM_H
