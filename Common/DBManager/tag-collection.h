#ifndef TAGCOLLECTION_H
#define TAGCOLLECTION_H

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
    QDomDocument data;
};

//----------------------------

struct TagItem
{
    QString name;
    QString path;

    TagCollection *collection = NULL;
    Tag *tag = NULL;
    TagFolder *folder = NULL;

    TagItem *parent = NULL;
    QList<TagItem *> subItems;

    bool expanded;
};

#endif // TAGCOLLECTION_H
