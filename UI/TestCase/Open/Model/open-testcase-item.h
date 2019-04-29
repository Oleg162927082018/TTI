#ifndef OPENTESTCASEITEM_H
#define OPENTESTCASEITEM_H

#include <QString>
#include <Common/DBManager/test-case.h>
#include <UI/TestCase/Open/Model/open-testcase-runitem.h>

struct OpenTestCaseItem
{
    bool isLoadResultsImediately;
    QString name;

    TestCase *testCase;
    QMap<int, OpenTestCaseRunItem *> fullRunDescriptions;
    QMap<int, OpenTestCaseRunItem *> visibleRunDescriptions;

    bool isDescriptionFilter;
    QString descriptionFilter;
    bool isFromDateTimeFilter;
    QDateTime fromDateTimeFilter;
    bool isToDateTimeFilter;
    QDateTime toDateTimeFilter;
};

#endif // OPENTESTCASEITEM_H
