#ifndef MAINWINDOWMODEL_H
#define MAINWINDOWMODEL_H

#include "Common/DBManager/test-case.h"
#include "Common/DBManager/test-case-cache.h"

#include <QList>


class MainWindowModel
{
public:
    MainWindowModel();

    static QList<TestCaseFolder *> tree;

    // OpenTestCase load test-case data and run data
    // but not load results of testing.
    //
    // Return:
    // pointer of test-case folder or NULL
    // errorCode = 0 - Success
    // errorCode = 1 - Test-case file not found
    // errorCode = 2 - Test-case already open
    static TestCaseFolder *OpenTestCase(TestCase *tc);
    static void UpdateRunDescriptions(TestCase *tc);

    // Load results of folder
    static void LoadFolderResults(TestCaseFolder *tcFolder);
    static void LoadRunResults(TestCaseFolder *folder, RunDescription *run);
    static void LoadTestResults(TestCaseFolder *folder, TestCaseItem *test);
    static void LoadOneResult(TestCaseFolder *folder, TestCaseItem *test, RunDescription *run);

private:
    static QList<TestCase *> testCaseList; //Private

    static void setTestCaseOwner(TestCaseFolder *folder, TestCase *tc);

};

#endif // MAINWINDOWMODEL_H
