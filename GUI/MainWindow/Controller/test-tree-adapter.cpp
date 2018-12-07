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

/*void TestTreeAdapter::setCheckState(MainWindowTreeFolder *folder, int checkState)
{
    folder->checkState = checkState;
    foreach (MainWindowTreeFolder *subfolder, folder->subFolders) {
        setCheckState(subfolder, checkState);
    }
}*/

/*void TestTreeAdapter::setParentCheckState(MainWindowTreeFolder *folder)
{
    MainWindowTreeFolder *parentFolder = folder->parentFolder;
    if(parentFolder != nullptr) {

        int checked_count = 0, unchecked_count = 0, partially_count = 0;

        foreach(MainWindowTreeFolder *subFolder, parentFolder->subFolders) {
            switch(subFolder->checkState) {
                case Qt::Checked: checked_count++;
                    break;
                case Qt::Unchecked: unchecked_count++;
                    break;
                case Qt::PartiallyChecked: partially_count++;
                    break;
            }
        }
        foreach(MainWindowTableItem *tableItem, parentFolder->fullTableItems) {
            if(tableItem->checked) { checked_count++; } else { unchecked_count++; }
        }

        int parentCheckState;
        if((partially_count == 0) && (unchecked_count == 0)) { parentCheckState = Qt::Checked; }
        else if((partially_count == 0) && (checked_count == 0)) { parentCheckState = Qt::Unchecked; }
        else { parentCheckState = Qt::PartiallyChecked; }

        if(parentFolder->checkState != parentCheckState) {
            parentFolder->checkState = parentCheckState;
            setParentCheckState(parentFolder);
        }
    }
}*/

