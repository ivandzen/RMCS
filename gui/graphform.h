#ifndef GRAPHFORM_H
#define GRAPHFORM_H

#include <QWidget>
#include <QHash>
#include <qcore/controller/qoparamcontroller.h>
#include "qcustomplot.h"

namespace Ui {
class GraphForm;
}

class FIRFilter
{
public:
    FIRFilter() { reset(); }

    FIRFilter(const QVector<float> & coeffs) :
        _coeffs(coeffs)
    {
        reset();
    }

    static FIRFilter sliderAverage(uint16_t size)
    {
        QVector<float> coeffs;
        coeffs.resize(size);
        for(int i = 0; i < size; ++i)
            coeffs[i] = 1.0f / float(size);
        return FIRFilter(coeffs);
    }

    inline void reset()
    {
        _values.resize(_coeffs.size());
        for(int i = 0; i < _coeffs.size(); ++i)
            _values[i] = 0.0;
        _currentIdx =0;
    }

    inline float exec(float value)
    {
        float result = 0.0;
        for(int i = 0; i < _coeffs.size(); ++i)
        {
            int value_idx = (_currentIdx + i) % _coeffs.size();
            result += _values[value_idx] * _coeffs[i];
        }
        _values[_currentIdx] = value;
        return result;
    }

private:
    QVector<float> _coeffs;
    QVector<float> _values;
    int _currentIdx;
};

class GraphForm : public QWidget
{
    Q_OBJECT

public:
    explicit GraphForm(QWidget *parent = 0);
    ~GraphForm();

    bool addOParam(QOParamController * param);

protected:
    virtual void timerEvent(QTimerEvent * event) override;

private:
    Ui::GraphForm *ui;

    class GraphSet : public QVector<QCPGraph*>
    {
    public:
        explicit GraphSet(ParamDataIdx max = 0) :
            _current(0),
            _max(max)
        {
            reserve(max);
        }

        inline QCPGraph * next() {
            QCPGraph * result = at(_current);
            if(++_current == _max)
                _current = 0;
            return result;
        }

    private:
        ParamDataIdx _current;
        ParamDataIdx _max;
    };

    QHash<QOParamController*, GraphSet> _graphs;

private slots:
    void onBufferFull();
};

#endif // GRAPHFORM_H
