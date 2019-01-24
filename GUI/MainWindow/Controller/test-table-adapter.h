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
    bool isInitNeeded(QMap<int, MainWindowTableItem*> *rowHeaders, QMap<int, MainWindowTableHeader *> *columnHeaders);
    void Init(QMap<int, MainWindowTableItem*> *rowHeaders, QMap<int, MainWindowTableHeader *> *columnHeaders);

    void beginResetModel();
    void endResetModel();
    void emitDataChanged();

    MainWindowTableItem* getRowData(int pos);
    int rowByData(MainWindowTableItem *data);
    int rowCount(const QModelIndex &parent) const;

private:
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    QMap<int, MainWindowTableItem*> *rowSource = nullptr;
    QMap<int, MainWindowTableHeader *> *headerSource = nullptr;
};

#endif // TESTTABLEADAPTER_H
