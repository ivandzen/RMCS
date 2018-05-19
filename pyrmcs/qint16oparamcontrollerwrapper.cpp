#include "qint16oparamcontrollerwrapper.h"
#include "PythonQt.h"

PyInt16OParamConsumer::PyInt16OParamConsumer(PyObject * callable,
                                             QObject *parent) :
    QObject(parent),
    _callable(callable)
{

}

void PyInt16OParamConsumer::execute(const _ParamData<int16_t> & data)
{
    QVariantList args;
    args.reserve(data.size() + 1);
    args.append(data.getID());
    for(ParamDataIdx i = 0; i < data.size(); ++i)
        args.append(data.get(i));

    PythonQt::self()->call(_callable, "execute", args);
}
