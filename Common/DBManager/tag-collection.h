#ifndef TAGCOLLECTION_H
#define TAGCOLLECTION_H

#include <QString>
#include <QList>
#include <QDomDocument>

struct TagCollection
{
    QString fullFileName;
    QString description;
};

struct TagFolder
{
    QString description;
};

struct Tag
{
    QString type;
    QString data;
};

//----------------------------

struct TagItem
{
    QString name;
    QString path;

    TagCollection *collection = nullptr;
    Tag *tag = nullptr;
    TagFolder *folder = nullptr;

    TagItem *parent = nullptr;
    QList<TagItem *> subItems;

    bool expanded;
};

#endif // TAGCOLLECTION_H
