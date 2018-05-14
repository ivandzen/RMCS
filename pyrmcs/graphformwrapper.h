#ifndef GRAPHFORMWRAPPER_H
#define GRAPHFORMWRAPPER_H
#include <gui/graphform.h>

class GraphFormWrapper :
        public QObject
{
    Q_OBJECT
public slots:
    GraphForm * new_GraphForm(QWidget * parent = nullptr) { return new GraphForm(parent); }

    void show(GraphForm * form) { form->show(); }

    bool addOParam(GraphForm * form, QOParamController * param) {
        return form->addOParam(param);
    }
};

#endif // GRAPHFORMWRAPPER_H
