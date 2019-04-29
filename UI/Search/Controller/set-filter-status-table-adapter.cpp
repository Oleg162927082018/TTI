#include "set-filter-status-table-adapter.h"

#include <QBrush>

SetFilterStatusTableAdapter::SetFilterStatusTableAdapter()
{

}

void SetFilterStatusTableAdapter::InitStatusCollection(SetFilterStatusCollection *statusCollection)
{
    statusSource = statusCollection;
}

void SetFilterStatusTableAdapter::InitHeaderCollection(QStringList *headerCollection)
{
    headerSource = headerCollection;
}

void SetFilterStatusTableAdapter::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void SetFilterStatusTableAdapter::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

void SetFilterStatusTableAdapter::emitDataChanged()
{
    QModelIndex topLeft = this->index(0,0);
    QModelIndex bottomRight = this->index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1);
    emit dataChanged(topLeft, bottomRight);
}

int SetFilterStatusTableAdapter::rowCount(const QModelIndex &parent) const
{
    if(statusSource != nullptr)
    {
        return statusSource->statuses.length();
    }
    else
    {
        return 0;
    }
}

void SetFilterStatusTableAdapter::changeCheckState()
{
    bool checkState = false;
    foreach(SetFilterStatusItem *item, statusSource->statuses) {
        if(!item->checked) {
            checkState = true;
            break;
        }
    }

    foreach(SetFilterStatusItem *item, statusSource->statuses) {
        item->checked = checkState;
    }

    emitDataChanged();
}

int SetFilterStatusTableAdapter::columnCount(const QModelIndex &parent) const
{
    if(headerSource != nullptr)
    {
        return headerSource->count() + 2;
    }
    else
    {
        return 0;
    }
}

QVariant SetFilterStatusTableAdapter::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::CheckStateRole) {

        if(index.column() == 0)
        {
            SetFilterStatusItem *status = statusSource->statuses.at(row);
            return (status->checked)?(Qt::Checked):(Qt::Unchecked);
        }

    } else if (role == Qt::DisplayRole) {

        if(col == 0)
        {
            SetFilterStatusItem *status = statusSource->statuses.at(row);
            return QVariant(status->label);
        }
        else if(col == 1)
        {
            SetFilterStatusItem *status = statusSource->statuses.at(row);
            return QVariant(status->description);
        }
        else if(col > 1)
        {
            return QVariant(0);
        }
    }

    if (role == Qt::BackgroundRole)
    {
        if(index.column() == 0)
        {
            SetFilterStatusItem *status = statusSource->statuses.at(row);
            return QVariant(status->color);
        }
        else
        {
            return QBrush(Qt::white);
        }
    }

    return QVariant();
}

QVariant SetFilterStatusTableAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return "Status";
                case 1:
                    return "Description";
                default:
                    return headerSource->at(section - 2);
            }
        }

        /*else if(orientation == Qt::Vertical)
        {
            return QString("V_" + QString::number(section));
        }*/
     }

    return QVariant();

}

Qt::ItemFlags SetFilterStatusTableAdapter::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractTableModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool SetFilterStatusTableAdapter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            SetFilterStatusItem *status = statusSource->statuses.at(index.row());

            status->checked = value.toBool();

            QModelIndex topLeft = this->index(0,0);
            QModelIndex bottomRight = this->index(-1, -1);
            emit this->dataChanged(topLeft, bottomRight);

            success = true;
        }
    }
    else
    {
        success = false;
    }

    return success;
}
