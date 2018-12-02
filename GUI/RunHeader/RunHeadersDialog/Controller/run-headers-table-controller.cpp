#include "run-headers-table-controller.h"

RunHeadersTableController::RunHeadersTableController(QObject *parent) : QAbstractTableModel(parent)
{
}

void RunHeadersTableController::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void RunHeadersTableController::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

void RunHeadersTableController::setDataSource(QMap<int, RunHeaderItem *> *runHeaderItems)
{
    if(dataSource != runHeaderItems) { dataSource = runHeaderItems; }
}

void RunHeadersTableController::setAllChecked()
{
    bool checkState = false;

    for(int i = 0; i < dataSource->values().length(); i++)
    {
        if(!dataSource->values().at(i)->checked)
        {
            checkState = true;
            break;
        }
    }

    QAbstractTableModel::beginResetModel();
    for(int i = 0; i < dataSource->values().length(); i++)
    {
        dataSource->values().at(i)->checked = checkState;
    }
    QAbstractTableModel::endResetModel();
}

int RunHeadersTableController::rowCount(const QModelIndex &parent) const
{
    if(dataSource != nullptr)
    {
        return dataSource->count();
    }

    return 0;
}

int RunHeadersTableController::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant RunHeadersTableController::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return "№";
                case 1:
                    return "Date/Time";
                case 2:
                    return "Description";
                default:
                    return "";
            }
        }

        /*else if(orientation == Qt::Vertical)
        {
            return QString(QString::number(section));
        }*/
     }

    return QVariant();
}

Qt::ItemFlags RunHeadersTableController::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractTableModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool RunHeadersTableController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            int pos = dataSource->keys().length() - index.row() - 1;
            int key = dataSource->keys().at(pos);
            RunHeaderItem *d = dataSource->value(key);
            d->checked = value.toBool();
            success = true;
        }
    }
    else
    {
        success = false;
    }

    return success;
}

QVariant RunHeadersTableController::data(const QModelIndex &index, int role) const
{
    if(dataSource != nullptr)
    {
        if (role == Qt::CheckStateRole) {

            if(index.column() == 0)
            {
                int pos = dataSource->keys().length() - index.row() - 1;
                int key = dataSource->keys().at(pos);
                RunHeaderItem *d = dataSource->value(key);
                return (d->checked)?(Qt::Checked):(Qt::Unchecked);
            }

        } else if (role == Qt::DisplayRole) {

            int pos = dataSource->keys().length() - index.row() - 1;
            int key = dataSource->keys().at(pos);
            if(index.column() == 0)
            {
                return QVariant(QString::number(dataSource->value(key)->rd->Num));
            }
            else if(index.column() == 1)
            {
                return QVariant(dataSource->value(key)->name);
            }
            else if(index.column() == 2)
            {
                return QVariant(dataSource->value(key)->rd->Comment);
            }
        }
    }

    return QVariant();
}
