#ifndef TAGCOLLECTIONTREEADAPTER_H
#define TAGCOLLECTIONTREEADAPTER_H

#include "TagManager/tagmanager.h"

#include <QAbstractItemModel>
#include <QList>

class TagCollectionTreeAdapter : public QAbstractItemModel
{
public:
    TagCollectionTreeAdapter();

    void beginResetModel();
    void endResetModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex indexByParth(QString path);

private:
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    int findRow(TagItem *tagItem) const;
};

#endif // TAGCOLLECTIONTREEADAPTER_H
