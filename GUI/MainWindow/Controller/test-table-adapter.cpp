#include "test-table-adapter.h"

#include <QBrush>

#include <GUI/MainWindow/Model/mainwindow-model.h>

TestTableAdapter::TestTableAdapter()
{

}

bool TestTableAdapter::isInitNeeded(QMap<int, MainWindowTableItem*> *rows, QMap<int, MainWindowTableHeader *> *headers)
{
    if((rowSource != rows) ||
            (headerSource != headers)) { return true; }
    else { return false; }
}

void TestTableAdapter::Init(QMap<int, MainWindowTableItem*> *rows, QMap<int, MainWindowTableHeader *> *headers)
{
    rowSource = rows;
    headerSource = headers;
}

void TestTableAdapter::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void TestTableAdapter::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

void TestTableAdapter::emitDataChanged()
{
    QModelIndex topLeft = this->index(0,0);
    QModelIndex bottomRight = this->index(rowCount(QModelIndex()) - 1, columnCount(QModelIndex()) - 1);
    emit dataChanged(topLeft, bottomRight);
}

MainWindowTableItem *TestTableAdapter::getRowData(int pos)
{
    if((0 <= pos) && (pos < rowSource->count())) {
        int key = rowSource->keys().at(pos);
        return rowSource->value(key);
    } else {
        return nullptr;
    }
}

int TestTableAdapter::rowByData(MainWindowTableItem *data)
{
    if(rowSource != nullptr)
    {
        int key = rowSource->key(data);
        return rowSource->keys().indexOf(key);
    }

    return -1;
}

int TestTableAdapter::rowCount(const QModelIndex &parent) const
{
    if(rowSource != nullptr)
    {
        return rowSource->count();
    }
    else
    {
        return 0;
    }
}

int TestTableAdapter::columnCount(const QModelIndex &parent) const
{
    if(headerSource != nullptr)
    {
        return headerSource->count() + 1;
    }
    else
    {
        return 0;
    }
}

QVariant TestTableAdapter::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    if (role == Qt::CheckStateRole) {

        if(index.column() == 0)
        {
            int key = rowSource->keys().at(row);
            MainWindowTableItem *test = rowSource->value(key);
            return (test->checked)?(Qt::Checked):(Qt::Unchecked);
        }

    } else if (role == Qt::DisplayRole) {

        if(col == 0)
        {
            int key = rowSource->keys().at(row);
            return QVariant(rowSource->value(key)->name);
        }
        else if(col > 0)
        {
            int runNum = headerSource->keys().at(col - 1);

            int key = rowSource->keys().at(row);
            MainWindowTableItem *test = rowSource->value(key);
            if(test->results.keys().contains(runNum)) {

                if(test->status->benchmarks.contains(runNum)) {
                    return QVariant(test->status->benchmarks.value(runNum).label + test->status->benchmarks.value(runNum).comment);
                } else {
                    return QVariant(test->results.value(runNum)->label + test->results.value(runNum)->comment);
                }

            } else { return QVariant(); }
        }
    }

    if (role == Qt::BackgroundRole)
    {
        if(index.column() == 0)
        {
            return QBrush(Qt::white);
        }
        else
        {
            int runNum = headerSource->keys().at(col - 1);
            int key = rowSource->keys().at(row);
            MainWindowTableItem *test = rowSource->value(key);
            if(test->results.keys().contains(runNum)) {

                if(test->status->benchmarks.contains(runNum)) {
                    return QVariant(QBrush(test->status->benchmarks.value(runNum).color));
                } else {
                    return QVariant(QBrush(test->results.value(runNum)->color));
                }
            }
            else { return QVariant(); }
        }
    }

    return QVariant();
}

QVariant TestTableAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return "Test Name";
                default:
                    int key = headerSource->keys().at(section - 1);
                    return headerSource->value(key)->name;
            }
        }

        /*else if(orientation == Qt::Vertical)
        {
            return QString("V_" + QString::number(section));
        }*/
     }

    return QVariant();
}

Qt::ItemFlags TestTableAdapter::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = this->QAbstractTableModel::flags(index);

    if(index.column() == 0)
    {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool TestTableAdapter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool success = true;

    if (index.isValid())
    {
        if (role == Qt::CheckStateRole)
        {
            int key = rowSource->keys().at(index.row());
            MainWindowTableItem *test = rowSource->value(key);

            MainWindowModel::setCheckState(test, value.toBool());

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
