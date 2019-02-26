#ifndef FILTERTAGCOLLECTIONTREEADAPTER_H
#define FILTERTAGCOLLECTIONTREEADAPTER_H

#include "TagManager/tagmanager.h"

#include <QAbstractItemModel>
#include <QList>

#include <GUI/Search/Model/set-filter-tag-item.h>

class SetFilterTagTreeAdapter : public QAbstractItemModel
{
public:
    SetFilterTagTreeAdapter();
    void InitDataSource(QList<SetFilterTagItem *> *dataSource);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

private:
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(SetFilterTagItem *item) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void setCheckState(SetFilterTagItem *tagItem);
    void setParentCheckState(SetFilterTagItem *tagItem);
    void setChildCheckState(SetFilterTagItem *tagItem, int checkState);
    void emitDataChanged();

    QList<SetFilterTagItem *> *dataSource = nullptr;
};

#endif // FILTERTAGCOLLECTIONTREEADAPTER_H
