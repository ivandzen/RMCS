#ifndef ISTREAMFORM_H
#define ISTREAMFORM_H
#include <qcore/controller/forms/nodecontrollerform.h>
#include <qcore/controller/qistreamcontroller.h>

namespace Ui {
class IStreamForm;
}

class IStreamForm :
        public NodeControllerForm
{
    Q_OBJECT

public:
    explicit IStreamForm(QIStreamController *stream,
                         QWidget *parent = 0);
    ~IStreamForm();

    inline QIStreamController * node() const {
        return reinterpret_cast<QIStreamController*>(NodeControllerForm::node());
    }

private:
    Ui::IStreamForm *ui;

private slots:
    void streamToggled(bool enabled);
    void on_streamEnabledBox_clicked(bool checked);
};

#endif // ISTREAMFORM_H
