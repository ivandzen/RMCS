#include "qdevicewindow.h"
#include "ui_qdevicewindow.h"
#include <QDebug>
#include <core/common/defaultnodes.h>

QDeviceWindow::QDeviceWindow(QDeviceController * device,
                             QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QDeviceWindow)
{
    ui->setupUi(this);
    _model.build(device);
    ui->treeView->setModel(&_model);
}

QDeviceWindow::~QDeviceWindow()
{
    delete ui;
}

void QDeviceWindow::on_treeView_clicked(const QModelIndex &index)
{
    QNodeController * node = _model.getNode(index);
    if(node == nullptr)
    {
        qDebug() << "tree item has nullptr pointer";
        return;
    }
    clearForms();
    createForms(node);
    node->update();
}

void QDeviceWindow::clearForms()
{
    ui->tableWidget->clear();
    //ui->listWidget->clear();
    for(NodeControllerForm * frm : _forms)
        frm->deleteLater();
    _forms.clear();
}

void QDeviceWindow::createForms(QNodeController *node)
{
    QObjectList children = node->children();

    int row_count = 0;
    for(QObject * obj : children)
        if(QNodeController * child = qobject_cast<QNodeController*>(obj))
        {
            if(isPropertyType(child->type()) || isParamType(child->type()))
                ++row_count;
        }
    ui->tableWidget->setRowCount(row_count);

    int table_row = 0;
    for(QObject * obj : children)
        if(QNodeController * child = qobject_cast<QNodeController*>(obj))
        {
            if(isPropertyType(child->type()) || isParamType(child->type()))
            {
                NodeControllerForm * form = _factory.createForm(child, this);
                if(form != 0)
                {
                    _forms.append(form);
                    QTableWidgetItem * name_item = new QTableWidgetItem(child->name());
                    ui->tableWidget->setItem(table_row, 0, name_item);

                    if(isROPropertyType(child->type()))
                    {
                        QTableWidgetItem * name_item = new QTableWidgetItem("RO_PROP");
                        ui->tableWidget->setItem(table_row, 1, name_item);
                    }
                    else if(isRWPropertyType(child->type()))
                    {
                        QTableWidgetItem * name_item = new QTableWidgetItem("RW_PROP");
                        ui->tableWidget->setItem(table_row, 1, name_item);
                    }
                    else if(isIParamType(child->type()))
                    {
                        QTableWidgetItem * name_item = new QTableWidgetItem("I_PARAM");
                        ui->tableWidget->setItem(table_row, 1, name_item);
                    }
                    else if(isOParamType(child->type()))
                    {
                        QTableWidgetItem * name_item = new QTableWidgetItem("O_PARAM");
                        ui->tableWidget->setItem(table_row, 1, name_item);
                    }


                    QTableWidgetItem * form_item = new QTableWidgetItem();
                    form_item->setSizeHint(QSize(100, form->minimumHeight()));
                    ui->tableWidget->setItem(table_row, 2, form_item);

                    ui->tableWidget->setCellWidget(table_row, 2, form);
                    ++table_row;
                }
            }
        }
}
