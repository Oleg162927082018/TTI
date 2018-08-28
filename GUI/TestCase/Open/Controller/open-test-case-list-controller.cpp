#include "open-test-case-list-controller.h"

#include <QStandardItem>

TestCaseListController::TestCaseListController(QObject* parent) : QAbstractListModel(parent)
{
}

void TestCaseListController::setDataSource(OpenTestCaseModel *model)
{
    dataSource = model;
}

int TestCaseListController::rowCount(const QModelIndex &parent) const
{
    if(dataSource != NULL)
    {
        return dataSource->testCaseList.count();
    }
    else
    {
        return 0;
    }
}

QVariant TestCaseListController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if ((role == Qt::DisplayRole) && (dataSource != NULL) &&
        (index.row() >= 0) && (index.row() < dataSource->testCaseList.length()))
    {
        return QVariant(dataSource->testCaseList.at(index.row())->Name);
    }
    else
    {
        return QVariant();
    }
}

void TestCaseListController::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}

void TestCaseListController::endResetModel()
{
    QAbstractListModel::endResetModel();
}
