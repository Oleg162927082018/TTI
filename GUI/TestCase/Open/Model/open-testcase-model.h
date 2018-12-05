#ifndef OPENTESTCASEMODEL_H
#define OPENTESTCASEMODEL_H

#include <QList>
#include <GUI/TestCase/Open/Model/open-testcase-item.h>

class OpenTestCaseModel
{
public:
    OpenTestCaseModel();

    void addTestCase(QString testCaseFileName);
    void removeTestCase(int ind);
    void clear();
    bool isTestCaseContained(QString testCaseFileName);

    QList<OpenTestCaseItem *> itemList;
    OpenTestCaseItem *curItem = nullptr;
};

#endif // OPENTESTCASEMODEL_H
