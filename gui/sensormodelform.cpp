#include "sensormodelform.h"
#include "ui_sensormodelform.h"
#include <Qt3DCore>
#include <QDebug>

typedef enum
{
    TELEMETRY_ACCELX = 0,
    TELEMETRY_ACCELY = 1,
    TELEMETRY_ACCELZ = 2,

    TELEMETRY_GYROX = 3,
    TELEMETRY_GYROY = 4,
    TELEMETRY_GYROZ = 5,

    TELEMETRY_MAGNETX = 6,
    TELEMETRY_MAGNETY = 7,
    TELEMETRY_MAGNETZ = 8
}
TelemetryIdx;

void SensorModel2D::execute(const _ParamData<int16_t> & data)
{
    if(_lastTimestamp != 0)
    {
        float dt = _timeCoeff * (data.getID() - _lastTimestamp);
        Qt3DCore::QTransform delta_rot;
        delta_rot.setRotationX(dt * _gyroXCoef * data.get(TELEMETRY_GYROX));
        delta_rot.setRotationX(dt * _gyroYCoef * data.get(TELEMETRY_GYROY));
        delta_rot.setRotationX(dt * _gyroZCoef * data.get(TELEMETRY_GYROZ));
        _tform.setMatrix(delta_rot.matrix() * _tform.matrix());
        qDebug() << _tform.rotationX();
    }
    _lastTimestamp = data.getID();
}

void SensorModel2D::paint(QPainter * painter,
                          const QStyleOptionGraphicsItem * option,
                          QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QVector3D vector = _tform.matrix() * QVector3D(10, 0, 0);
    painter->drawLine(QPointF(0.0, 0.0),
                      QPointF(vector.x(), vector.y()));
}

SensorModelForm::SensorModelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorModelForm)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    startTimer(30);
}

SensorModelForm::~SensorModelForm()
{
    delete ui;
}

void SensorModelForm::addModel(SensorModel2D * sensor)
{
    ui->graphicsView->scene()->addItem(sensor);
}

void SensorModelForm::timerEvent(QTimerEvent * event)
{
    Q_UNUSED(event);
    ui->graphicsView->update();
}
