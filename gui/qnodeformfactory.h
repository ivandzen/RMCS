#ifndef QNODEFORMFACTORY_H
#define QNODEFORMFACTORY_H
#include <gui/qnodeform.h>

class QNodeFormFactory
{
public:
    QNodeForm * createForm(QNodeController * controller,
                                    QWidget *parent);
};

#endif // QNODEFORMFACTORY_H
