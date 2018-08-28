#ifndef TESTTABLEADAPTER_H
#define TESTTABLEADAPTER_H

#include "Common/DBManager/test-case-cache.h"
#include "Common/DBManager/run-description.h"

#include <QAbstractTableModel>

class TestTableAdapter : public QAbstractTableModel
{
public:
    TestTableAdapter();
    void Init(QList<TestCaseItem*> *rowHeaders, QMap<int, RunDescription *> *columnHeaders);

    void beginResetModel();
    void endResetModel();

private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<TestCaseItem*> *rowHeaderSource = 0;
    QMap<int, RunDescription *> *columnHeaderSource = 0;
};

#endif // TESTTABLEADAPTER_H
