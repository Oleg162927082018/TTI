#include "open-testcase-rundescriptions-controller.h"

OpenTestCaseRunDescriptionsController::OpenTestCaseRunDescriptionsController(QObject *parent) : QAbstractTableModel(parent)
{
}

void OpenTestCaseRunDescriptionsController::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void OpenTestCaseRunDescriptionsController::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

void OpenTestCaseRunDescriptionsController::setAllChecked()
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

    //QAbstractTableModel::beginResetModel();

    for(int i = 0; i < dataSource->values().length(); i++)
    {
        dataSource->values().at(i)->checked = checkState;
    }

    QModelIndex leftTop = this->index(0, 0);
    QModelIndex bottomRight = this->index(rowCount(QModelIndex()) - 1, 0);
    emit this->dataChanged(leftTop, bottomRight);
    //QAbstractTableModel::endResetModel();
}

int OpenTestCaseRunDescriptionsController::rowCount(const QModelIndex &parent) const
{
    if(dataSource != nullptr)
    {
        return dataSource->count();
    }

    return 0;
}

int OpenTestCaseRunDescriptionsController::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant OpenTestCaseRunDescriptionsController::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags OpenTestCaseRunDescriptionsController::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractTableModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool OpenTestCaseRunDescriptionsController::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            int pos = dataSource->keys().length() - index.row() - 1;
            int key = dataSource->keys().at(pos);
            OpenTestCaseRunItem *d = dataSource->value(key);
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

QVariant OpenTestCaseRunDescriptionsController::data(const QModelIndex &index, int role) const
{
    if(dataSource != nullptr)
    {
        if (role == Qt::CheckStateRole) {

            if(index.column() == 0)
            {
                int pos = dataSource->keys().length() - index.row() - 1;
                int key = dataSource->keys().at(pos);
                OpenTestCaseRunItem *d = dataSource->value(key);
                return (d->checked)?(Qt::Checked):(Qt::Unchecked);
            }

        } else if (role == Qt::DisplayRole) {

            int pos = dataSource->keys().length() - index.row() - 1;
            if(index.column() == 0)
            {
                int key = dataSource->keys().at(pos);
                return QVariant(QString::number(dataSource->value(key)->runDescription->Num));
            }
            else if(index.column() == 1)
            {
                int key = dataSource->keys().at(pos);
                return QVariant(dataSource->value(key)->name);
            }
            else if(index.column() == 2)
            {
                int key = dataSource->keys().at(pos);
                return QVariant(dataSource->value(key)->runDescription->Comment);
            }
        }
    }

    return QVariant();
}
