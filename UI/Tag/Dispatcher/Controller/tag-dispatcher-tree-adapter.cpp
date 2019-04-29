#include "tag-dispatcher-tree-adapter.h"

#include <UI/Tag/Dispatcher/Model/tag-dispatcher-item.h>

TagDispatcherTreeAdapter::TagDispatcherTreeAdapter()
{
}

void TagDispatcherTreeAdapter::InitDataSource(QList<TagDispatcherItem *> *dataSource)
{
    this->dataSource = dataSource;
}

void TagDispatcherTreeAdapter::beginResetModel()
{
    QAbstractItemModel::beginResetModel();
}

void TagDispatcherTreeAdapter::endResetModel()
{
    QAbstractItemModel::endResetModel();
}

QModelIndex TagDispatcherTreeAdapter::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) { return QModelIndex(); }

    if (!parent.isValid())
    {
        TagDispatcherItem *tagCollection = dataSource->at(row);
        return createIndex(row, column, tagCollection);
    }
    else
    {
        TagDispatcherItem *parentItem = static_cast<TagDispatcherItem *>(parent.internalPointer());
        return createIndex(row, column, parentItem->subItems.at(row));
    }
}

QVariant TagDispatcherTreeAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return QVariant(); }

    if (role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            TagDispatcherItem *tagItem = static_cast<TagDispatcherItem *>(index.internalPointer());
            if(tagItem->collection != nullptr) { return QVariant(tagItem->collection->name); }
            else if(tagItem->folder != nullptr) { return QVariant(tagItem->folder->name); }
            else if(tagItem->tag != nullptr) { return QVariant(tagItem->tag->name); }
        }
    }
    return QVariant();
}

int TagDispatcherTreeAdapter::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return dataSource->length();
    }
    else
    {
        TagDispatcherItem *parentItem = static_cast<TagDispatcherItem *>(parent.internalPointer());
        return parentItem->subItems.length();
    }
}

int TagDispatcherTreeAdapter::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex TagDispatcherTreeAdapter::parent(const QModelIndex &child) const
{
    if (!child.isValid()) { return QModelIndex(); }

    TagDispatcherItem *tagItem = static_cast<TagDispatcherItem *>(child.internalPointer());
    if (tagItem->parent != nullptr)
    {
        return createIndex(findRow(tagItem->parent), 0, tagItem->parent);
    }
    else
    {
        return QModelIndex();
    }
}

int TagDispatcherTreeAdapter::findRow(TagDispatcherItem *tagItem) const
{
    if(tagItem->parent == nullptr)
    {
        return dataSource->indexOf(tagItem);
    }
    else
    {
        for(int i = 0; i < tagItem->parent->subItems.length(); i++ )
        {
            if(tagItem->parent->subItems.at(i) == tagItem) { return i; }
        }

        return 0;
    }
}
