#include "set-filter-dialog-model.h"

#include "Common/DllManager/dllmanager.h"
#include <TagManager/tagmanager.h>

SetFilterDialogModel::SetFilterDialogModel()
{

}

SetFilterDialogModel::~SetFilterDialogModel()
{
    foreach(SetFilterTagItem *item, tagCollections) { delete item; }

    foreach(SetFilterStatusCollection *statusCollection, statusCollectionList) {
        foreach(SetFilterStatusItem *item, statusCollection->statuses) { delete item; }
        delete statusCollection;
    }
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

void SetFilterDialogModel::appendUpdateTestCase(TestCase *tc)
{
    const int MAX_STATUS_COUNT = 9999;

    ITestOutputComparator *tcComparator = nullptr;
    ITestCaseAdapter *tcAdapter = DLLManager::GetTestCaseAdapter(tc->ID);
    if(tcAdapter != nullptr) { tcComparator = tcAdapter->GetComparator(); }

    SetFilterStatusCollection *item = nullptr;

    int ind = testCaseFileNameList.indexOf(tc->fullFileName);
    if(ind < 0) {
        testCaseFileNameList.append(tc->fullFileName);
        item = new SetFilterStatusCollection();
        statusCollectionList.append(item);

        if(tcComparator != nullptr) {
            for(int i = 0; i < MAX_STATUS_COUNT; i++) {
                SetFilterStatusItem *status = new SetFilterStatusItem();
                if(tcComparator->GetTestStatus(i, status->label, status->color, status->description)) {
                    item->statuses.append(status);
                } else {
                    delete status;
                    break;
                }
            }
        }
    } else {
        item = statusCollectionList[ind];
        int i = 0;

        if(tcComparator != nullptr) {

            SetFilterStatusItem *status;
            for(i = 0; i < MAX_STATUS_COUNT; i++) {
                if(i < item->statuses.length()) {
                    status = item->statuses[i];
                } else {
                    status = new SetFilterStatusItem();
                    item->statuses.append(status);
                }

                if(!tcComparator->GetTestStatus(i, status->label, status->color, status->description)) {
                    break;
                }
            }
        }

        int removeCount = item->statuses.length() - i;
        for(i = 0; i < removeCount; i++)
        {
            delete item->statuses.last();
            item->statuses.removeLast();
        }
    }

    item->name = tc->fullFileName.mid(tc->fullFileName.lastIndexOf("/") + 1);

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
