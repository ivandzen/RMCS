#ifndef QDEVICEMODEL_H
#define QDEVICEMODEL_H
#include <QAbstractItemModel>
#include <core/common/types.h>

class QDeviceController;
class QNodeController;
class QDeviceModelItem;

class QDeviceModel :
        public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit QDeviceModel(QObject * parent = nullptr);

    void build(QDeviceController * dev);

    virtual ~QDeviceModel();

    virtual QVariant data(const QModelIndex &index, int role) const override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                       int role = Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column,
                                  const QModelIndex &parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex &index) const override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QNodeController * getNode(const QModelIndex & index) const;

private:
    void buildBranch(QDeviceModelItem * parent,
                     QDeviceController * dev);

    QVector<QDeviceModelItem*>  _topLevelItems;
};

#endif // QDEVICEMODEL_H
