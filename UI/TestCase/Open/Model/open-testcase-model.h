#ifndef OPENTESTCASEMODEL_H
#define OPENTESTCASEMODEL_H

#include <QList>
#include <UI/TestCase/Open/Model/open-testcase-item.h>

class OpenTestCaseModel
{
public:
    OpenTestCaseModel();

    void addTestCase(QString testCaseFileName);
    void removeTestCase(int ind);
    void clear();
    bool isTestCaseContained(QString testCaseFileName);

    QList<OpenTestCaseItem *> itemList;
};

#endif // OPENTESTCASEMODEL_H
