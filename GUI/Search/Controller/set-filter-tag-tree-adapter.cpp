#include "set-filter-tag-tree-adapter.h"

#include <QDomDocument>

SetFilterTagTreeAdapter::SetFilterTagTreeAdapter()
{

}

void SetFilterTagTreeAdapter::InitDataSource(QList<SetFilterTagItem *> *dataSource)
{
    this->dataSource = dataSource;
}

QModelIndex SetFilterTagTreeAdapter::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) { return QModelIndex(); }

    if (!parent.isValid())
    {
        SetFilterTagItem *tagCollection = dataSource->at(row);
        return createIndex(row, column, tagCollection);
    }
    else
    {
        SetFilterTagItem *parentItem = static_cast<SetFilterTagItem *>(parent.internalPointer());
        return createIndex(row, column, parentItem->subItems.at(row));
    }
}

QVariant SetFilterTagTreeAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return QVariant(); }

    if (role == Qt::CheckStateRole) {

        if(index.column() == 0)
        {
            SetFilterTagItem* tagItem = static_cast<SetFilterTagItem*>(index.internalPointer());
            return tagItem->checkState;
        }

    } else if (role == Qt::DisplayRole) {

        if(index.column() == 0)
        {
            SetFilterTagItem *tagItem = static_cast<SetFilterTagItem *>(index.internalPointer());
            if(tagItem->collection != nullptr) { return QVariant(tagItem->collection->name); }
            else if(tagItem->folder != nullptr) { return QVariant(tagItem->folder->name); }
            else if(tagItem->tag != nullptr) { return QVariant(tagItem->tag->name); }
        }

    }
    return QVariant();
}

int SetFilterTagTreeAdapter::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return dataSource->length();
    }
    else
    {
        SetFilterTagItem *parentItem = static_cast<SetFilterTagItem *>(parent.internalPointer());
        return parentItem->subItems.length();
    }
}

int SetFilterTagTreeAdapter::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex SetFilterTagTreeAdapter::parent(const QModelIndex &child) const
{
    if (!child.isValid()) { return QModelIndex(); }

    SetFilterTagItem *tagItem = static_cast<SetFilterTagItem *>(child.internalPointer());
    if (tagItem->parent != nullptr)
    {
        return createIndex(findRow(tagItem->parent), 0, tagItem->parent);
    }
    else
    {
        return QModelIndex();
    }
}

int SetFilterTagTreeAdapter::findRow(SetFilterTagItem *tagItem) const
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

Qt::ItemFlags SetFilterTagTreeAdapter::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractItemModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsTristate;
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool SetFilterTagTreeAdapter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            SetFilterTagItem* folder = static_cast<SetFilterTagItem*>(index.internalPointer());
            setCheckState(folder);
            success = true;
        }
    }
    else
    {
        success = false;
    }

    return success;
}

void SetFilterTagTreeAdapter::setChildCheckState(SetFilterTagItem *tagItem, int checkState)
{
    tagItem->checkState = checkState;
    foreach (SetFilterTagItem *subTagItem, tagItem->subItems) {
        setChildCheckState(subTagItem, checkState);
    }
}

void SetFilterTagTreeAdapter::setParentCheckState(SetFilterTagItem *tagItem)
{
    if(tagItem != nullptr) {

        int checked_count = 0, unchecked_count = 0, partially_count = 0;

        foreach(SetFilterTagItem *subTagItem, tagItem->subItems) {
            switch(subTagItem->checkState) {
                case Qt::Checked: checked_count++;
                    break;
                case Qt::Unchecked: unchecked_count++;
                    break;
                case Qt::PartiallyChecked: partially_count++;
                    break;
            }
        }

        int parentCheckState;
        if((partially_count == 0) && (unchecked_count == 0)) { parentCheckState = Qt::Checked; }
        else if((partially_count == 0) && (checked_count == 0)) { parentCheckState = Qt::Unchecked; }
        else { parentCheckState = Qt::PartiallyChecked; }

        if(tagItem->checkState != parentCheckState) {
            tagItem->checkState = parentCheckState;
            setParentCheckState(tagItem->parent);
        }
    }
}

void SetFilterTagTreeAdapter::setCheckState(SetFilterTagItem *tagItem)
{
    bool isChecked = tagItem->checkState != Qt::Checked;

    setChildCheckState(tagItem, isChecked?Qt::Checked:Qt::Unchecked);
    setParentCheckState(tagItem->parent);

    emitDataChanged();
}

void SetFilterTagTreeAdapter::emitDataChanged()
{
    QModelIndex topLeft = this->index(0,0);
    QModelIndex bottomRight = this->index(-1, -1);
    emit this->dataChanged(topLeft, bottomRight);
}
