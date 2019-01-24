#ifndef TAGCOLLECTION_H
#define TAGCOLLECTION_H

#include <QString>
#include <QList>

struct TagCollection;
struct TagFolder;

struct Tag
{
    QString name;

    QString type;
    QString data;

    TagCollection *owner;
    TagFolder *parent;
};

struct TagFolder
{
    QString name;
    QString description;

    QList<TagFolder *> folders;
    QList<Tag *> tags;

    TagCollection *owner;
    TagFolder *parent;

    ~TagFolder() {
        foreach(Tag *tag, tags) { delete tag; }
        foreach(TagFolder *folder, folders) { delete folder; }
    }
};

struct TagCollection: TagFolder
{
    QString fullFileName;
};

#endif // TAGCOLLECTION_H
