#include "open-test-case-list-controller.h"

#include <QStandardItem>

OpenTestCaseListController::OpenTestCaseListController(QObject* parent) : QAbstractListModel(parent)
{
}

void OpenTestCaseListController::emitDataChanged()
{
    QModelIndex leftTop = this->index(0);
    QModelIndex bottomRight = this->index(rowCount(QModelIndex()) - 1);
    emit this->dataChanged(leftTop, bottomRight);
}

int OpenTestCaseListController::rowCount(const QModelIndex &parent) const
{
    if(dataSource != nullptr)
    {
        return dataSource->count();
    }
    else
    {
        return 0;
    }
}

QVariant OpenTestCaseListController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if ((role == Qt::DisplayRole) && (dataSource != nullptr) &&
        (index.row() >= 0) && (index.row() < dataSource->count()))
    {
        return QVariant(dataSource->at(index.row())->name);
    }
    else
    {
        return QVariant();
    }
}
