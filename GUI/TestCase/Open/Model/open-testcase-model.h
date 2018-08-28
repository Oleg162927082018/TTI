#ifndef OPENTESTCASEMODEL_H
#define OPENTESTCASEMODEL_H

#include "Common/DBManager/dbmanager.h"

#include <QList>

class OpenTestCaseModel
{
public:
    OpenTestCaseModel();

    void addTestCase(QString testCaseFileName);
    void removeTestCase(int ind);
    void clear();
    bool isTestCaseContained(QString testCaseFileName);

    QList<TestCase *> testCaseList;
    TestCase *currentTestCase = NULL;
};

#endif // OPENTESTCASEMODEL_H
