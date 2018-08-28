#include <QBrush>

#include "test-table-adapter.h"

TestTableAdapter::TestTableAdapter()
{

}

void TestTableAdapter::Init(QList<TestCaseItem*> *rowHeaders, QMap<int, RunDescription *> *columnHeaders)
{
    rowHeaderSource = rowHeaders;
    columnHeaderSource = columnHeaders;
}

void TestTableAdapter::beginResetModel()
{
    QAbstractTableModel::beginResetModel();
}

void TestTableAdapter::endResetModel()
{
    QAbstractTableModel::endResetModel();
}

int TestTableAdapter::rowCount(const QModelIndex &parent) const
{
    if(rowHeaderSource != NULL)
    {
        return rowHeaderSource->count();
    }
    else
    {
        return 0;
    }
}

int TestTableAdapter::columnCount(const QModelIndex &parent) const
{
    if(columnHeaderSource != 0)
    {
        return columnHeaderSource->count() + 1;
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
            return QVariant(rowHeaderSource->at(row)->Name);
        }
        else if(col > 0)
        {

            int runNum = columnHeaderSource->keys().at(col - 1);

            TestCaseItem *test = rowHeaderSource->at(row);
            if(test->status->benchmarks.contains(runNum))
            {
                return QVariant("Benchmark updated");
            }
            else
            {
                TestResult *r = test->results.value(runNum);
                if(r != NULL)
                {
                    return QVariant(r->status);
                }
                else
                {
                    return QVariant();
                }
            }
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
            int runNum = columnHeaderSource->keys().at(col - 1);
            TestCaseItem *test = rowHeaderSource->at(row);
            if(test->status->benchmarks.contains(runNum))
            {
                return QVariant(QColor("#008080"));
            }
            else
            {
                TestResult *r = test->results.value(runNum);
                if(r != NULL)
                {
                    return QBrush(r->color);
                }
                else
                {
                    return QVariant();
                }
            }
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
                    int key = columnHeaderSource->keys().at(section - 1);
                    return columnHeaderSource->value(key)->LocalDateTimeOfStart.toString(Qt::SystemLocaleShortDate);
            }
        }

        /*else if(orientation == Qt::Vertical)
        {
            return QString("V_" + QString::number(section));
        }*/
     }

     return QVariant();
}
