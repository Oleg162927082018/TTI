#ifndef TESTCASETREEADAPTER_H
#define TESTCASETREEADAPTER_H

#include "GUI/MainWindow/Model/mainwindow-treefolder.h"

#include <QAbstractItemModel>
#include <QList>

class TestTreeAdapter : public QAbstractItemModel
{
public:
    TestTreeAdapter();

    void beginResetModel();
    void endResetModel();
    void emitDataChanged();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

private:
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(MainWindowTreeFolder *testFolder) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // TESTCASETREEADAPTER_H
