#include "qrmcsscriptengine.h"
#include <QDebug>

QRmcsScriptEngine::QRmcsScriptEngine(QObject * parent) :
    QJSEngine(parent),
    _rmcs(new QRmcs(this))
{
    QJSValue rmcs_value = newQObject(_rmcs);
    globalObject().setProperty("rmcs", rmcs_value);
}

void QRmcsScriptEngine::runComand(const QString &com)
{
    QJSValue result = evaluate(com);
    if(result.isError())
        qDebug() << "Uncaught JS engine exeption";
    else
        qDebug() << result.toString();
}
