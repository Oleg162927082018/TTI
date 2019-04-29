#ifndef TAGDISPATCHERMODEL_H
#define TAGDISPATCHERMODEL_H

#include "tag-dispatcher-item.h"

#include <QList>



class TagDispatcherModel
{
public:
    TagDispatcherModel();
    ~TagDispatcherModel();

    void Init();

    TagDispatcherItem *NewTagCollection(QString fullFileName, QString description);
    TagDispatcherItem *NewTagFolder(TagDispatcherItem *parent, QString name, QString description);
    TagDispatcherItem *NewTag(TagDispatcherItem *parent, QString id, QString name, QString data);

    void DeleteTagItem(TagDispatcherItem *tagItem, bool isPermanently);

    QList<TagDispatcherItem *> tagCollections;

private:
    void InitTagItem(TagDispatcherItem *item, TagFolder *sourceFolder);

};

#endif // TAGDISPATCHERMODEL_H
