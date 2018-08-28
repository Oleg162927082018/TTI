#include "log-adapter.h"

LogAdapter::LogAdapter(QObject *parent)
    : QAbstractListModel(parent)
{
}

void LogAdapter::Init(QStringList *d)
{
    dataSource = d;
}

void LogAdapter::beginResetModel()
{
    QAbstractListModel::beginResetModel();
}

void LogAdapter::endResetModel()
{
    QAbstractListModel::endResetModel();
}

int LogAdapter::rowCount(const QModelIndex &parent) const
{
    if(dataSource != NULL)
    {
        return dataSource->count();
    }

    return 0;
}

QVariant LogAdapter::data(const QModelIndex &index, int role) const
{
    if((role == Qt::DisplayRole) && (dataSource != NULL))
    {
        int row = index.row();
        return QVariant(dataSource->at(row));
    }

    return QVariant();
}
