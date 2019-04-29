#ifndef TAGCOLLECTIONTREEADAPTER_H
#define TAGCOLLECTIONTREEADAPTER_H

#include "TagManager/tagmanager.h"

#include <QAbstractItemModel>
#include <QList>

#include <UI/Tag/Dispatcher/Model/tag-dispatcher-model.h>
#include <UI/Tag/Dispatcher/Model/tag-dispatcher-item.h>

class TagDispatcherTreeAdapter : public QAbstractItemModel
{
public:
    TagDispatcherTreeAdapter();
    void InitDataSource(QList<TagDispatcherItem *> *dataSource);

    void beginResetModel();
    void endResetModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

private:
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(TagDispatcherItem *tagItem) const;

    QList<TagDispatcherItem *> *dataSource = nullptr;
};

#endif // TAGCOLLECTIONTREEADAPTER_H
