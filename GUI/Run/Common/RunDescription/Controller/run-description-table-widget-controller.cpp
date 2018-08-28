#include "run-description-table-widget-controller.h"

RunDescriptionTableWidgetController::RunDescriptionTableWidgetController(QObject *parent) : QAbstractTableModel(parent)
{
}

void RunDescriptionTableWidgetController::setDataSource(QMap<int, RunDescription *> *runDescriptionList)
{
    if(dataSource != runDescriptionList)
    {
        QAbstractTableModel::beginResetModel();
        dataSource = runDescriptionList;
        QAbstractTableModel::endResetModel();
    }
}

void RunDescriptionTableWidgetController::setAllChecked()
{
    bool checkState = false;

    for(int i = 0; i < dataSource->values().length(); i++)
    {
        if(!dataSource->values().at(i)->Checked)
        {
            checkState = true;
            break;
        }
    }

    QAbstractTableModel::beginResetModel();
    for(int i = 0; i < dataSource->values().length(); i++)
    {
        dataSource->values().at(i)->Checked = checkState;
    }
    QAbstractTableModel::endResetModel();
}

int RunDescriptionTableWidgetController::rowCount(const QModelIndex &parent) const
{
    if(dataSource != NULL)
    {
        return dataSource->count();
    }

    return 0;
}

int RunDescriptionTableWidgetController::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant RunDescriptionTableWidgetController::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags RunDescriptionTableWidgetController::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractTableModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool RunDescriptionTableWidgetController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            int key = dataSource->keys().at(index.row());
            RunDescription *d = dataSource->value(key);
            d->Checked = value.toBool();
            success = true;
        }
    }
    else
    {
        success = false;
    }

    return success;
}

QVariant RunDescriptionTableWidgetController::data(const QModelIndex &index, int role) const
{
    if(dataSource != NULL)
    {
        if (role == Qt::CheckStateRole) {

            if(index.column() == 0)
            {
                int key = dataSource->keys().at(index.row());
                RunDescription *d = dataSource->value(key);
                return (d->Checked)?(Qt::Checked):(Qt::Unchecked);
            }

        } else if (role == Qt::DisplayRole) {

            if(index.column() == 0)
            {
                int key = dataSource->keys().at(index.row());
                return QVariant(QString::number(dataSource->value(key)->Num));
            }
            else if(index.column() == 1)
            {
                int key = dataSource->keys().at(index.row());
                return QVariant(dataSource->value(key)->LocalDateTimeOfStart.toString("dd-MMM-yyyy HH:mm:ss"));
            }
            else if(index.column() == 2)
            {
                int key = dataSource->keys().at(index.row());
                return QVariant(dataSource->value(key)->Comment);
            }
        }
    }

    return QVariant();
}
