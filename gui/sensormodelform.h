#ifndef SENSORMODELFORM_H
#define SENSORMODELFORM_H

#include <QWidget>
#include <qcore/controller/qoparamcontroller.h>
#include <QGraphicsObject>
#include <Qt3DCore>

namespace Ui {
class SensorModelForm;
}

class SensorModel2D :
        public QGraphicsObject,
        public QInt16OParamController::Consumer
{
    Q_OBJECT
public:
    SensorModel2D(QGraphicsItem * parent = nullptr) :
        QGraphicsObject(parent)
    {}

protected:
    virtual void execute(const _ParamData<int16_t> & data) override;

    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget = Q_NULLPTR) override;

private:
    Qt3DCore::QTransform _tform;
    float           _gyroXCoef      = 1.0f / 65536.0f;
    float           _gyroYCoef      = 1.0f / 65536.0f;
    float           _gyroZCoef      = 1.0f / 65536.0f;
    float           _timeCoeff      = 0.1f;
    ParamDataID     _lastTimestamp  = 0;
};

class SensorModelForm : public QWidget
{
    Q_OBJECT

public:
    explicit SensorModelForm(QWidget *parent = 0);
    ~SensorModelForm();

    void addModel(SensorModel2D * sensor);

protected:
    virtual void timerEvent(QTimerEvent * event) final override;

private:
    Ui::SensorModelForm *ui;
};

#endif // SENSORMODELFORM_H
