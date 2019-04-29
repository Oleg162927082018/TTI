#ifndef SETFILTERDIALOGMODEL_H
#define SETFILTERDIALOGMODEL_H

#include "set-filter-status-collection.h"
#include "set-filter-tag-item.h"

#include <Common/DBManager/test-case.h>

class SetFilterDialogModel
{
public:
    SetFilterDialogModel();
    ~SetFilterDialogModel();

    void Init();
    QStringList *getCheckedTagLinks();

    QList<SetFilterTagItem *> tagCollections;

//Status collection
public:
    QStringList testCaseFileNameList;
    QList<SetFilterStatusCollection *> statusCollectionList;
    QStringList statusHeaderList;

    void appendUpdateTestCase(TestCase *tc);

private:
    void InitTagItem(SetFilterTagItem *item, TagFolder *sourceFolder);
    void getCheckedTagLinks(SetFilterTagItem *tagItem, QStringList *checkedTagLinks);

};

#endif // SETFILTERDIALOGMODEL_H
