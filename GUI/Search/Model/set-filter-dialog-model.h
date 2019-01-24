#ifndef SETFILTERDIALOGMODEL_H
#define SETFILTERDIALOGMODEL_H

#include "set-filter-tag-item.h"

class SetFilterDialogModel
{
public:
    SetFilterDialogModel();
    ~SetFilterDialogModel();

    void Init();
    QStringList *getCheckedTagLinks();

    QList<SetFilterTagItem *> tagCollections;

private:
    void InitTagItem(SetFilterTagItem *item, TagFolder *sourceFolder);
    void getCheckedTagLinks(SetFilterTagItem *tagItem, QStringList *checkedTagLinks);

};

#endif // SETFILTERDIALOGMODEL_H
