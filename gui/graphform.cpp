#include "graphform.h"
#include "ui_graphform.h"
#include <QDebug>

GraphForm::GraphForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphForm)
{
    ui->setupUi(this);
}

GraphForm::~GraphForm()
{
    delete ui;
}

bool GraphForm::addOParam(QOParamController * param)
{
    if(_graphs.contains(param))
        return false;

    QCPGraph * newGraph = ui->plot->addGraph();
    newGraph->setPen(QPen(Qt::GlobalColor(rand() % (int)Qt::darkYellow)));
    _graphs.insert(param, newGraph);
    _filters.insert(param, FIRFilter::sliderAverage(1));
    connect(param, SIGNAL(bufferFull()), this, SLOT(onBufferFull()));
    return true;
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

    QCPGraph * graph = _graphs[param];
    FIRFilter & filter = _filters[param];
    if(graph == nullptr)
    {
        qDebug() << "GraphForm::onBufferFull  Nullptr граф";
        return;
    }

    bool found;
    QCPRange xrange = graph->getKeyRange(found);
    double x = 0;
    if(found)
        x = xrange.upper;

    param->map([this, graph, &filter, &x](const QVariant & value) {
        bool ok =false;
        double fvalue = value.toDouble(&ok);
        if(!ok)
        {
            qDebug() << "GraphForm::onBufferFull unable to convert value to float";
            return;
        }
        fvalue = double(filter.exec(fvalue));
        if(fvalue != 0.0)
            graph->addData(x, fvalue);
        x += 1;
    });

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

    param->reset();

    ui->plot->replot();
}
