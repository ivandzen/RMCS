#ifndef QNODEFORM_H
#define QNODEFORM_H

#include <QWidget>

class QNodeController;

class QNodeForm : public QWidget
{
    Q_OBJECT
public:
    explicit QNodeForm(QNodeController * node,
                                QWidget *parent = 0);

    inline QNodeController * node() const { return _node; }

private:
    QNodeController * _node;
};

#endif // QNODEFORM_H
