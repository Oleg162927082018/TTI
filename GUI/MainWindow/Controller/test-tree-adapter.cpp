#include "test-tree-adapter.h"

#include "GUI/MainWindow/Model/mainwindow-model.h"

TestTreeAdapter::TestTreeAdapter()
{

}

void TestTreeAdapter::beginResetModel()
{
    QAbstractItemModel::beginResetModel();
}

void TestTreeAdapter::endResetModel()
{
    QAbstractItemModel::endResetModel();
}

void TestTreeAdapter::emitDataChanged()
{
    QModelIndex topLeft = this->index(0,0);
    QModelIndex bottomRight = this->index(-1, -1);
    emit this->dataChanged(topLeft, bottomRight);
}

QModelIndex TestTreeAdapter::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) { return QModelIndex(); }

    if (!parent.isValid())
    {
        return createIndex(row, column,
                           const_cast<MainWindowTreeFolder*>(MainWindowModel::tree.at(row)));
    }
    else
    {
        MainWindowTreeFolder* parentFolder = static_cast<MainWindowTreeFolder*>(parent.internalPointer());
        return createIndex(row, column, parentFolder->subFolders.at(row));
    }
}

QVariant TestTreeAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return QVariant(); }

    if (role == Qt::CheckStateRole) {

        if(index.column() == 0)
        {
            MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
            return folder->checkState;
        }
    } else if (role == Qt::DisplayRole) {

        if(index.column() == 0)
        {
            MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
            return QVariant(folder->name);
        }
    }
    return QVariant();
}

int TestTreeAdapter::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return MainWindowModel::tree.length();
    }
    else
    {
        MainWindowTreeFolder* parentFolder = static_cast<MainWindowTreeFolder*>(parent.internalPointer());
        return parentFolder->subFolders.count();
    }
}

int TestTreeAdapter::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex TestTreeAdapter::parent(const QModelIndex &index) const
{
    if (!index.isValid()) { return QModelIndex(); }

    MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
    if (folder->parentFolder != nullptr)
    {
        return createIndex(findRow(folder->parentFolder), 0, folder->parentFolder);
    }
    else
    {
        return QModelIndex();
    }
}

int TestTreeAdapter::findRow(MainWindowTreeFolder *treeFolder) const
{
    if(treeFolder->parentFolder == nullptr)
    {
        return MainWindowModel::tree.indexOf(treeFolder);
    }
    else
    {
        return treeFolder->parentFolder->subFolders.indexOf(treeFolder);
    }
}

Qt::ItemFlags TestTreeAdapter::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractItemModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsTristate;
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool TestTreeAdapter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
            MainWindowModel::setCheckState(folder);
            success = true;
        }
    }
    else
    {
        success = false;
    }

    return success;
}
