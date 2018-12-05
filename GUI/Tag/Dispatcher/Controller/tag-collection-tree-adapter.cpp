#include "tag-collection-tree-adapter.h"

#include <QDomDocument>

TagCollectionTreeAdapter::TagCollectionTreeAdapter()
{

}

void TagCollectionTreeAdapter::beginResetModel()
{
    QAbstractItemModel::beginResetModel();
}

void TagCollectionTreeAdapter::endResetModel()
{
    QAbstractItemModel::endResetModel();
}

QModelIndex TagCollectionTreeAdapter::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) { return QModelIndex(); }

    if (!parent.isValid())
    {
        TagItem *tagCollection = TagManager::GetTagCollection(row);
        return createIndex(row, column, const_cast<TagItem *>(tagCollection));
    }
    else
    {
        TagItem *parentItem = static_cast<TagItem *>(parent.internalPointer());
        return createIndex(row, column, parentItem->subItems.at(row));
    }
}

QVariant TagCollectionTreeAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return QVariant(); }

    if (role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            TagItem *tagItem = static_cast<TagItem *>(index.internalPointer());
            return QVariant(tagItem->name);
        }
    }
    return QVariant();
}

int TagCollectionTreeAdapter::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return TagManager::GetTagCollectionCount();
    }
    else
    {
        TagItem *parentItem = static_cast<TagItem *>(parent.internalPointer());
        return parentItem->subItems.length();
    }
}

int TagCollectionTreeAdapter::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex TagCollectionTreeAdapter::parent(const QModelIndex &child) const
{
    if (!child.isValid()) { return QModelIndex(); }

    TagItem *tagItem = static_cast<TagItem *>(child.internalPointer());
    if (tagItem->parent != nullptr)
    {
        return createIndex(findRow(tagItem->parent), 0, tagItem->parent);
    }
    else
    {
        return QModelIndex();
    }
}

int TagCollectionTreeAdapter::findRow(TagItem *tagItem) const
{
    if(tagItem->parent == nullptr)
    {
        return TagManager::GetTagCollectionInd(tagItem->name);
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

QModelIndex TagCollectionTreeAdapter::indexByParth(QString path)
{
    QStringList itemList = path.split(':', QString::SkipEmptyParts);

    int row = TagManager::GetTagCollectionInd(itemList.at(0));
    if(row == -1) { return QModelIndex(); }
    TagItem *item = TagManager::GetTagCollection(row);

    for(int i = 1; i < itemList.length(); i++)
    {
        row = -1;
        for(int j = 0; j < item->subItems.length(); j++)
        {
            if(item->subItems.at(j)->name.compare(itemList.at(i), Qt::CaseInsensitive) == 0 )
            {
                row = j;
                item = item->subItems.at(row);
            }
        }
        if(row == -1) { return QModelIndex(); }
    }

    return createIndex(row, 0, item);
}
