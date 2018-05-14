#include "qdevicemodel.h"
#include <QVariant>
#include <qcore/controller/qnodecontroller.h>
#include <qcore/controller/qdevicecontroller.h>
#include <core/common/defaultnodes.h>

class QDeviceModelItem
{
public :
    explicit QDeviceModelItem(QNodeController * node,
                              QDeviceModelItem * parent) :
        _node(node),
        _parent(parent)
    {
        if(_parent)
            _parent->_children.push_back(this);
    }

    virtual ~QDeviceModelItem()
    {
        for(QDeviceModelItem * child_node : _children)
            delete child_node;
    }

    inline QNodeController * node() const { return _node; }

    QVariant data(int) { return QVariant(_node->name()); }

    int childCount() const { return _children.size(); }

    QDeviceModelItem * parentItem() const { return _parent; }

    int row() const
    {
        if (_parent)
            return _parent->_children.indexOf(const_cast<QDeviceModelItem*>(this));
        return 0;
    }

    inline QDeviceModelItem * child(int index) const { return _children[index]; }

private:
    QNodeController *           _node;
    QDeviceModelItem *          _parent;
    QVector<QDeviceModelItem*>  _children;
};

//////////////////////////////////////////////////////////////////////////////

inline bool isSystemNode(QNodeController * node)
{
    NodeType_t type = node->type();
    return /*isParamType(type) || */isPropertyType(type);
}

QDeviceModel::QDeviceModel(QObject * parent) :
    QAbstractItemModel (parent)
{

}

void QDeviceModel::build(QDeviceController * dev)
{
    for(NodeID_t id = 0; id < dev->numNodes(); ++id)
    {
        QNodeController * node = dev->getNode(id);
        if((node->parentId() == NodeIDNull) && (!isSystemNode(node)))
        {
            QDeviceModelItem * item = new QDeviceModelItem(node, nullptr);
            buildBranch(item, dev);
            _topLevelItems.push_back(item);
        }
    }
}

QDeviceModel::~QDeviceModel()
{
    for(QDeviceModelItem * child : _topLevelItems)
        delete child;
}

QVariant QDeviceModel::data(const QModelIndex & index,
                            int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    QDeviceModelItem * item = static_cast<QDeviceModelItem*>(index.internalPointer());

    return item->data(role);
}

Qt::ItemFlags QDeviceModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant QDeviceModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const
{
    (void)section;

    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
        return QVariant();

    return QVariant();
}

QModelIndex QDeviceModel::index(int row,
                                int column,
                                const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    QDeviceModelItem * parentItem;

    if(!parent.isValid())
        return createIndex(row, column, _topLevelItems[row]);
        //parentItem = _topLevelItems[row];
    else
        parentItem = static_cast<QDeviceModelItem*>(parent.internalPointer());

    QDeviceModelItem * childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex QDeviceModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QDeviceModelItem * childItem = static_cast<QDeviceModelItem*>(index.internalPointer());
    QDeviceModelItem * parentItem = childItem->parentItem();

    if(parentItem == nullptr)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int QDeviceModel::rowCount(const QModelIndex &parent) const
{
    QDeviceModelItem * parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return _topLevelItems.size();
    else
        parentItem = static_cast<QDeviceModelItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int QDeviceModel::columnCount(const QModelIndex & parent) const
{
    (void)parent;
    return 1;
}

QNodeController *QDeviceModel::getNode(const QModelIndex &index) const
{
    QDeviceModelItem * item = static_cast<QDeviceModelItem*>(index.internalPointer());
    if(item)
        return  item->node();
    return nullptr;
}

void QDeviceModel::buildBranch(QDeviceModelItem * parent,
                               QDeviceController * dev)
{
    QObjectList children = parent->node()->children();
    for(QObject * obj : children)
        if(QNodeController * child = qobject_cast<QNodeController*>(obj))
        {
            if(isSystemNode(child))
                continue;
            QDeviceModelItem * new_item = new QDeviceModelItem(child, parent);
            buildBranch(new_item, dev);
        }
}
