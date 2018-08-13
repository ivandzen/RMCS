#ifndef QINT16OPARAMCONTROLLERWRAPPER_H
#define QINT16OPARAMCONTROLLERWRAPPER_H
#include <qcore/controller/qoparamcontroller.h>
#include "qoparamcontrollerwrapper.h"
#include "PythonQt.h"

class PyInt16OParamConsumer :
        public QObject,
        public QInt16OParamController::Consumer
{
    Q_OBJECT
public:
    explicit PyInt16OParamConsumer(PyObject * callable,
                                   QObject * parent = nullptr);

    virtual void execute(const _ParamData<int16_t> & data) override;

private:
    PyObject * _callable;
};

class QInt16OParamControllerWrapper :
        public QOParamControllerWrapper
{
    Q_OBJECT
public slots:
    QInt16OParamController * new_QInt16OParamController(QNodeController * channel) {
        //! @attention here is no type checking
        return reinterpret_cast<QInt16OParamController*>(channel);
    }

    void setConsumer(QInt16OParamController * oparam,
                     PyObject * callable) {
        oparam->setConsumer(new PyInt16OParamConsumer(callable, this));
    }
};

Q_DECLARE_METATYPE(PyObject*);

#endif // QINT16OPARAMCONTROLLERWRAPPER_H
