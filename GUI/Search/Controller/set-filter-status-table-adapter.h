#ifndef SETFILTERSTATUSCOLLECTIONADAPTER_H
#define SETFILTERSTATUSCOLLECTIONADAPTER_H

#include <QAbstractTableModel>

#include <GUI/Search/Model/set-filter-status-collection.h>

class SetFilterStatusTableAdapter : public QAbstractTableModel
{
public:
    SetFilterStatusTableAdapter();

    void InitStatusCollection(SetFilterStatusCollection *statusCollection);
    void InitHeaderCollection(QStringList *headerCollection);

    void beginResetModel();
    void endResetModel();
    void emitDataChanged();

    int rowCount(const QModelIndex &parent) const;

    void changeCheckState();

private:
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private:
    SetFilterStatusCollection *statusSource = nullptr;
    QStringList *headerSource = nullptr;
};

#endif // SETFILTERSTATUSCOLLECTIONADAPTER_H
