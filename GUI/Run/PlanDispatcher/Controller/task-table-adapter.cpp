#include "task-table-adapter.h"

#include "Common/RunManager/runmanager.h"

TaskTableAdapter::TaskTableAdapter(QObject *parent)
    : QAbstractTableModel(parent)
{
}

void TaskTableAdapter::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void TaskTableAdapter::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

QVariant TaskTableAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return "Test Plan";
                case 1:
                    return "Description";
                case 2:
                    return "Status";
                default:
                    return QVariant();
            }
        }
        else if(orientation == Qt::Vertical)
        {
            return QString(QString::number(section + 1));
        }
     }

     return QVariant();
}

int TaskTableAdapter::rowCount(const QModelIndex &parent) const
{
    return RunManager::GetQueueLength();
}

int TaskTableAdapter::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant TaskTableAdapter::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::DisplayRole)
    {
        if(col == 0)
        {
            return QVariant(RunManager::GetPlan(row)->fullFileName);
        }
        else if(col == 1)
        {
            return QVariant(RunManager::GetPlan(row)->description);
        }
        else if(col == 2)
        {
            int status = RunManager::GetPlan(row)->status;
            switch(status)
            {
                case PlanStatus::Wating: return QVariant("Wating");
                case PlanStatus::Prosessing: return QVariant("Processing...");
                case PlanStatus::Finished: return QVariant("Finished");
                case PlanStatus::Pause: return QVariant("Pause");
                default: return QVariant();
            }
        }
        else if(col > 2)
        {
            return QVariant();
        }
    }

    return QVariant();
}
