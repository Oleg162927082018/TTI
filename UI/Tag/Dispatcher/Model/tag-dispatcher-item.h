#ifndef TAGVIEWITEM_H
#define TAGVIEWITEM_H

#include <QList>

#include <Common/DBManager/tag-collection.h>


struct TagDispatcherItem
{
    bool expanded = false;

    TagCollection *collection = nullptr;
    TagFolder *folder = nullptr;
    Tag *tag = nullptr;

    TagDispatcherItem *parent = nullptr;
    QList<TagDispatcherItem *> subItems;

    ~TagDispatcherItem() {
        foreach(TagDispatcherItem *subItem, subItems) { delete subItem; }
    }
};

#endif // TAGVIEWITEM_H
