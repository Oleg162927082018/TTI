#include "test-table-adapter.h"

#include <QBrush>

TestTableAdapter::TestTableAdapter()
{

}

bool TestTableAdapter::isInitNeeded(QList<MainWindowTableItem*> *rows, QMap<int, MainWindowTableHeader *> *headers)
{
    if((rowSource != rows) ||
            (headerSource != headers)) { return true; }
    else { return false; }
}

void TestTableAdapter::Init(QList<MainWindowTableItem*> *rows, QMap<int, MainWindowTableHeader *> *headers)
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

MainWindowTableItem *TestTableAdapter::getRowData(int pos)
{
    if((0 <= pos) && (pos < rowSource->count())) {
        return rowSource->at(pos);
    } else {
        return nullptr;
    }
}

int TestTableAdapter::rowByData(MainWindowTableItem *data)
{
    if(rowSource != nullptr)
    {
        return rowSource->indexOf(data);
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

    if (role == Qt::DisplayRole)
    {
        if(col == 0)
        {
            return QVariant(rowSource->at(row)->name);
        }
        else if(col > 0)
        {
            int runNum = headerSource->keys().at(col - 1);

            MainWindowTableItem *test = rowSource->at(row);
            if(test->results.keys().contains(runNum)) {

                if(test->status->benchmarks.contains(runNum)) {
                    return QVariant("Benchmark updated");
                } else {
                    return QVariant(test->results.value(runNum)->status);
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
            MainWindowTableItem *test = rowSource->at(row);
            if(test->results.keys().contains(runNum)) {

                if(test->status->benchmarks.contains(runNum)) {
                    return QVariant(QColor("#008080"));
                } else {
                    return QBrush(test->results.value(runNum)->color);
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
