#ifndef QRMCSSCRIPTENGINE_H
#define QRMCSSCRIPTENGINE_H
#include <QJSEngine>
#include <qcore/controller/supervisor.h>

class QRmcsScriptEngine :
        public QJSEngine
{
public:
    explicit QRmcsScriptEngine(QObject * parent = nullptr);

    void runComand(const QString & com);

private:
    QRmcs * _rmcs;
};

#endif // QRMCSSCRIPTENGINE_H
