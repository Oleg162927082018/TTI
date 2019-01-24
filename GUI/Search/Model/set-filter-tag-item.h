#ifndef SETFILTERTAGITEM_H
#define SETFILTERTAGITEM_H

#include <QString>

#include <Common/DBManager/tag-collection.h>

struct SetFilterTagItem
{
    int checkState = Qt::Unchecked;
    QString name;

    TagCollection *collection = nullptr;
    TagFolder *folder = nullptr;
    Tag *tag = nullptr;

    SetFilterTagItem *parent = nullptr;
    QList<SetFilterTagItem *> subItems;

    ~SetFilterTagItem() {
        foreach(SetFilterTagItem *subItem, subItems) { delete subItem; }
    }
};

#endif // SETFILTERTAGITEM_H
