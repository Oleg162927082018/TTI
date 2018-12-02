#ifndef TESTTABLEADAPTER_H
#define TESTTABLEADAPTER_H

#include "GUI/MainWindow/Model/mainwindow-tableheader.h"
#include "GUI/MainWindow/Model/mainwindow-tableitem.h"

#include <QAbstractTableModel>

class TestTableAdapter : public QAbstractTableModel
{
public:
    TestTableAdapter();

    //Return true if new rowHeaders and new columnHeaders was setted up
    bool isInitNeeded(QList<MainWindowTableItem*> *rowHeaders, QMap<int, MainWindowTableHeader *> *columnHeaders);
    void Init(QList<MainWindowTableItem*> *rowHeaders, QMap<int, MainWindowTableHeader *> *columnHeaders);

    void beginResetModel();
    void endResetModel();

    MainWindowTableItem* getRowData(int pos);

private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    QList<MainWindowTableItem*> *rowSource = nullptr;
    QMap<int, MainWindowTableHeader *> *headerSource = nullptr;
};

#endif // TESTTABLEADAPTER_H
