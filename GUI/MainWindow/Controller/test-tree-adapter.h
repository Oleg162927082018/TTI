#ifndef TESTCASETREEADAPTER_H
#define TESTCASETREEADAPTER_H

#include "Common/DBManager/test-case-cache.h"

#include <QAbstractItemModel>
#include <QList>

class TestTreeAdapter : public QAbstractItemModel
{
public:
    TestTreeAdapter();

    void beginResetModel();
    void endResetModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

private:
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(TestCaseFolder *testFolder) const;
};

#endif // TESTCASETREEADAPTER_H
