#include "tag-dispatcher-model.h"

#include <TagManager/tagmanager.h>

TagDispatcherModel::TagDispatcherModel()
{

}

TagDispatcherModel::~TagDispatcherModel()
{
    foreach(TagDispatcherItem *item, tagCollections) { delete item; }
}

void TagDispatcherModel::Init()
{
    for(int i = 0; i < TagManager::GetTagCollectionCount(); i++) {
        TagDispatcherItem *item = new TagDispatcherItem();
        item->collection = TagManager::GetTagCollection(i);

        InitTagItem(item, item->collection);

        tagCollections.append(item);
    }
}

void TagDispatcherModel::InitTagItem(TagDispatcherItem *item, TagFolder *sourceFolder)
{
    foreach(TagFolder *subFolder,sourceFolder->folders) {
        TagDispatcherItem *subItem = new TagDispatcherItem();
        subItem->folder = subFolder;
        subItem->parent = item;
        item->subItems.append(subItem);

        InitTagItem(subItem, subFolder);
    }

    foreach(Tag *tag,sourceFolder->tags) {
        TagDispatcherItem *subItem = new TagDispatcherItem();
        subItem->tag = tag;
        subItem->parent = item;
        item->subItems.append(subItem);
    }
}

TagDispatcherItem *TagDispatcherModel::NewTagCollection(QString fullFileName, QString description)
{
    TagDispatcherItem *item = new TagDispatcherItem();
    item->collection = TagManager::NewTagCollection(fullFileName, description);
    tagCollections.append(item);

    return item;
}

TagDispatcherItem *TagDispatcherModel::NewTagFolder(TagDispatcherItem *parent, QString name, QString description)
{
    TagDispatcherItem *item = new TagDispatcherItem();
    if(parent->collection != nullptr) {
        item->folder = TagManager::NewTagFolder(parent->collection, name, description);
    } else if(parent->folder != nullptr) {
        item->folder = TagManager::NewTagFolder(parent->folder, name, description);
    }

    item->parent = parent;
    parent->subItems.append(item);

    return item;
}

TagDispatcherItem *TagDispatcherModel::NewTag(TagDispatcherItem *parent, QString id, QString name, QString data)
{
    TagDispatcherItem *item = new TagDispatcherItem();
    if(parent->collection != nullptr) {
        item->tag = TagManager::NewTag(parent->collection, id, name, data);
    } else if(parent->folder != nullptr) {
        item->tag = TagManager::NewTag(parent->folder, id, name, data);
    }

    item->parent = parent;
    parent->subItems.append(item);

    return item;
}

void TagDispatcherModel::DeleteTagItem(TagDispatcherItem *tagItem, bool isPermanently)
{
    //Delete tag
    if(tagItem->tag != nullptr)
    {
        TagManager::DeleteTag(tagItem->tag);
        tagItem->parent->subItems.removeOne(tagItem);
        delete tagItem;
    }
    //Delete Folder with tags
    else if(tagItem->folder != nullptr)
    {
        TagManager::DeleteTagFolder(tagItem->folder);
        tagItem->parent->subItems.removeOne(tagItem);
        delete tagItem;
    }
    //Delete Collection
    else if(tagItem->collection != nullptr)
    {
        TagManager::DeleteTagCollection(tagItem->collection, isPermanently);
        tagCollections.removeOne(tagItem);
        delete tagItem;
    }
}
