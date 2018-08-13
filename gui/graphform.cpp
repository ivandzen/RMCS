#include "graphform.h"
#include "ui_graphform.h"
#include <QDebug>

GraphForm::GraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphForm)
{
    ui->setupUi(this);
    startTimer(30);
}

GraphForm::~GraphForm()
{
    delete ui;
}

bool GraphForm::addOParam(QOParamController * param)
{
    if(_graphs.contains(param))
        return false;

    GraphSet newSet(param->count());

    for(int i = 0; i < param->count(); ++i)
    {
        QCPGraph * newGraph = ui->plot->addGraph();
        newGraph->setPen(QPen(Qt::GlobalColor(rand() % (int)Qt::darkYellow)));
        newSet.append(newGraph);
    }

    _graphs.insert(param, newSet);
    /*
    param->setHandler([this](QOParamController * param,
                             ParamDataID id,
                             const QVector<QVariant> & values)
    {
        GraphSet & set = _graphs[param];

        for(int i = 0; i < values.size(); ++i)
        {
            QCPGraph * graph = set[i];

            if(graph == nullptr)
            {
                qDebug() << "GraphForm::onBufferFull  Nullptr граф";
                return;
            }

            bool found;
            QCPRange xrange = graph->getKeyRange(found);
            double x = id;
            x /= 3.0;
            //if(found)
            //    x = xrange.upper + 1;

            bool ok =false;
            double fvalue = values[i].toDouble(&ok);
            if(!ok)
            {
                qDebug() << "GraphForm::onBufferFull unable to convert value to float";
                return;
            }

            //if(fvalue != 0.0)
            graph->addData(x, fvalue);

            xrange = graph->getKeyRange(found);
            if(!found)
            {
                qDebug() << "GraphForm::onBufferFull  Key range for graph not found. Exit";
                return;
            }

            QCPRange yrange = graph->getValueRange(found);
            if(!found)
            {
                qDebug() << "GraphForm::onBufferFull  Value range for graph not found. Exit";
                return;
            }


            if(xrange.upper > ui->plot->xAxis->range().upper)
            {
                ui->plot->xAxis->setRangeUpper(xrange.upper);
                ui->plot->xAxis->setRangeLower(ui->plot->xAxis->range().upper - 2000);
            }

            //if(xrange.lower < ui->plot->xAxis->range().lower)


            if(yrange.lower < ui->plot->yAxis->range().lower)
                ui->plot->yAxis->setRangeLower(yrange.lower);

            if(yrange.upper > ui->plot->yAxis->range().upper)
                ui->plot->yAxis->setRangeLower(yrange.upper);
        }
    });
    */
    //connect(param, SIGNAL(bufferFull()), this, SLOT(onBufferFull()));
    return true;
}

void GraphForm::timerEvent(QTimerEvent *event)
{
    ui->plot->replot();
}

void GraphForm::onBufferFull()
{
    QOParamController * param = qobject_cast<QOParamController*>(QObject::sender());
    if(param == nullptr)
    {
        qDebug() << "GraphForm::onBufferFull  Сендер не является QOParamController";
        return;
    }

    if(!_graphs.contains(param))
    {
        qDebug() << "GraphForm::onBufferFull  Граф для истоника сигнала не найден";
        return;
    }

    GraphSet & set = _graphs[param];

    /*
    param->map([this, &set](const QVariant & value)
    {
        QCPGraph * graph = set.next();

        if(graph == nullptr)
        {
            qDebug() << "GraphForm::onBufferFull  Nullptr граф";
            return;
        }

        bool found;
        QCPRange xrange = graph->getKeyRange(found);
        double x = 0;
        if(found)
            x = xrange.upper + 1;

        bool ok =false;
        double fvalue = value.toDouble(&ok);
        if(!ok)
        {
            qDebug() << "GraphForm::onBufferFull unable to convert value to float";
            return;
        }

        //if(fvalue != 0.0)
        graph->addData(x, fvalue);

        xrange = graph->getKeyRange(found);
        if(!found)
        {
            qDebug() << "GraphForm::onBufferFull  Key range for graph not found. Exit";
            return;
        }

        QCPRange yrange = graph->getValueRange(found);
        if(!found)
        {
            qDebug() << "GraphForm::onBufferFull  Value range for graph not found. Exit";
            return;
        }


        if(xrange.upper > ui->plot->xAxis->range().upper)
        {
            ui->plot->xAxis->setRangeUpper(xrange.upper);
            ui->plot->xAxis->setRangeLower(ui->plot->xAxis->range().upper - 2000);
        }

        //if(xrange.lower < ui->plot->xAxis->range().lower)


        if(yrange.lower < ui->plot->yAxis->range().lower)
            ui->plot->yAxis->setRangeLower(yrange.lower);

        if(yrange.upper > ui->plot->yAxis->range().upper)
            ui->plot->yAxis->setRangeLower(yrange.upper);
    });

    param->reset();
    */

    ui->plot->replot();
}
