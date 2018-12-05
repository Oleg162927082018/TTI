#ifndef RUNHEADERSDIALOGITEM_H
#define RUNHEADERSDIALOGITEM_H

#include "Common/DBManager/run-description.h"

struct RunHeaderItem
{
    bool checked;
    QString name;
    RunDescription *rd = nullptr;
};

#endif // RUNHEADERSDIALOGITEM_H
