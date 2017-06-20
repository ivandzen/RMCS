#ifndef QNODEFORMFACTORY_H
#define QNODEFORMFACTORY_H
#include <gui/nodecontrollerform.h>

class QNodeFormFactory
{
public:
    NodeControllerForm * createForm(QNodeController * controller,
                                    QWidget *parent);
};

#endif // QNODEFORMFACTORY_H
