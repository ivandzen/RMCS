#ifndef NODECONTROLLERFORM_H
#define NODECONTROLLERFORM_H

#include <QWidget>

class QNodeController;

class NodeControllerForm : public QWidget
{
    Q_OBJECT
public:
    explicit NodeControllerForm(QNodeController * node,
                                QWidget *parent = 0);

    inline QNodeController * node() const { return _node; }

private:
    QNodeController * _node;
};

#endif // NODECONTROLLERFORM_H
