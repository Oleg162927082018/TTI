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
                           const_cast<TestCaseFolder*>(MainWindowModel::tree.at(row)));
    }
    else
    {
        TestCaseFolder* parentFolder = static_cast<TestCaseFolder*>(parent.internalPointer());
        return createIndex(row, column, parentFolder->SubFolderList.at(row));
    }
}

QVariant TestTreeAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) { return QVariant(); }

    if (role == Qt::DisplayRole)
    {
        if(index.column() == 0)
        {
            TestCaseFolder* folder = static_cast<TestCaseFolder*>(index.internalPointer());
            return QVariant(folder->Name);
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
        TestCaseFolder* parentInfo = static_cast<TestCaseFolder*>(parent.internalPointer());
        return parentInfo->SubFolderList.count();
    }
}

int TestTreeAdapter::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex TestTreeAdapter::parent(const QModelIndex &index) const
{
    if (!index.isValid()) { return QModelIndex(); }

    TestCaseFolder* folder = static_cast<TestCaseFolder*>(index.internalPointer());
    if (folder->ParentFolder != 0)
    {
        return createIndex(findRow(folder->ParentFolder), 0, folder->ParentFolder);
    }
    else
    {
        return QModelIndex();
    }
}

int TestTreeAdapter::findRow(TestCaseFolder *testFolder) const
{
    if(testFolder->ParentFolder == NULL)
    {
        return MainWindowModel::tree.indexOf(testFolder);
    }
    else
    {
        return testFolder->ParentFolder->SubFolderList.indexOf(testFolder);
    }
}
