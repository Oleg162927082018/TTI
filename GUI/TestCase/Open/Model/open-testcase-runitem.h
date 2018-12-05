#ifndef OPENTESTCASERUNITEM_H
#define OPENTESTCASERUNITEM_H

#include <QString>
#include <Common/DBManager/run-description.h>

struct OpenTestCaseRunItem
{
    bool checked;
    QString name;

    RunDescription *runDescription;
};

#endif // OPENTESTCASERUNITEM_H
