#include "set-filter-dialog-model.h"

#include <TagManager/tagmanager.h>

SetFilterDialogModel::SetFilterDialogModel()
{

}

SetFilterDialogModel::~SetFilterDialogModel()
{
    foreach(SetFilterTagItem *item, tagCollections) { delete item; }
}

void SetFilterDialogModel::Init()
{
    for(int i = 0; i < TagManager::GetTagCollectionCount(); i++) {
        SetFilterTagItem *item = new SetFilterTagItem();
        item->collection = TagManager::GetTagCollection(i);

        InitTagItem(item, item->collection);

        tagCollections.append(item);
    }
}

QStringList *SetFilterDialogModel::getCheckedTagLinks()
{
    QStringList *checkedTagLinks = new QStringList();
    foreach(SetFilterTagItem *item, tagCollections) { getCheckedTagLinks(item, checkedTagLinks);  }
    return checkedTagLinks;
}

void SetFilterDialogModel::getCheckedTagLinks(SetFilterTagItem *tagItem, QStringList *checkedTagLinks)
{
    if(tagItem->tag != nullptr) {
        if(tagItem->checkState == Qt::Checked) {
            checkedTagLinks->append(TagManager::GetTagLink(tagItem->tag));
        }
    } else {
        foreach(SetFilterTagItem *subItem, tagItem->subItems) { getCheckedTagLinks(subItem, checkedTagLinks);  }
    }
}

void SetFilterDialogModel::InitTagItem(SetFilterTagItem *item, TagFolder *sourceFolder)
{
    foreach(TagFolder *subFolder,sourceFolder->folders) {
        SetFilterTagItem *subItem = new SetFilterTagItem();
        subItem->folder = subFolder;
        subItem->parent = item;
        item->subItems.append(subItem);

        InitTagItem(subItem, subFolder);
    }

    foreach(Tag *tag,sourceFolder->tags) {
        SetFilterTagItem *subItem = new SetFilterTagItem();
        subItem->tag = tag;
        subItem->parent = item;
        item->subItems.append(subItem);
    }
}
