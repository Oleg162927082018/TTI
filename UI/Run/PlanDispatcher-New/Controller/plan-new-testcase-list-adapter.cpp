#include "plan-new-testcase-list-adapter.h"

PlanNewTestCaseListAdapter::PlanNewTestCaseListAdapter(QObject *parent)
    : QAbstractListModel(parent)
{

}

void PlanNewTestCaseListAdapter::Init(QStringList *d)
{
    dataSource = d;
}

void PlanNewTestCaseListAdapter::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}

void PlanNewTestCaseListAdapter::endResetModel()
{
    QAbstractListModel::endResetModel();
}

int PlanNewTestCaseListAdapter::rowCount(const QModelIndex &parent) const
{
    if(dataSource != nullptr)
    {
        return dataSource->count();
    }

    return 0;
}

QVariant PlanNewTestCaseListAdapter::data(const QModelIndex &index, int role) const
{
    if((role == Qt::DisplayRole) && (dataSource != nullptr))
    {
        int row = index.row();
        return QVariant(dataSource->at(row));
    }

    return QVariant();
}
