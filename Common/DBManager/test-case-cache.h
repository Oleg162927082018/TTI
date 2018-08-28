#ifndef TESTCASETREE_H
#define TESTCASETREE_H

#include <QList>
#include "test-case.h"
#include "test-result.h"
#include "test-status.h"

struct TestCaseItem
{
    QString Name;
    QString RelativeFileName; //Name from folder STATUS

    //Links
    bool Checked;
    TestCase *ownerTestCase = NULL;
    QMap<int, TestResult *> results;
    TestStatus *status = NULL;
};

struct TestCaseFolder
{
    QString Name;
    TestCaseFolder *ParentFolder = 0;
    QList<TestCaseFolder *> SubFolderList;
    QList<TestCaseItem *> TestList;

    //Links
    bool Checked;
    bool Expanded;
    TestCase *ownerTestCase = NULL;
};

#endif // TESTCASETREE_H
