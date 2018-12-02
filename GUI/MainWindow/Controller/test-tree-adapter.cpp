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

    if (role == Qt::DisplayRole)
    {
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
