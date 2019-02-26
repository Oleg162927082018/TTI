#include "set-filter-testcase-list-adapter.h"

SetFilterTestCaseListAdapter::SetFilterTestCaseListAdapter()
{

}

void SetFilterTestCaseListAdapter::Init(QList<SetFilterStatusCollection *> *d)
{
    dataSource = d;
}

int SetFilterTestCaseListAdapter::rowCount(const QModelIndex &parent) const
{
    return dataSource->count();
}

QVariant SetFilterTestCaseListAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (index.row() >= 10) return QVariant();

    if (role == Qt::DisplayRole) {
        SetFilterStatusCollection *item = dataSource->at(index.row());
        return QVariant(item->name);
    } else {
        return QVariant();
    }
}
