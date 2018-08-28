#include "mainwindow-model.h"

#include "Common/DBManager/dbmanager.h"

#include <QFileInfo>

QList<TestCaseFolder *> MainWindowModel::tree;

QList<TestCase *> MainWindowModel::testCaseList;

MainWindowModel::MainWindowModel()
{

}

TestCaseFolder *MainWindowModel::OpenTestCase(TestCase *tc)
{
    //Check test-case already open
    for(int i = 0; i < testCaseList.length(); i++)
    {
        QString fullFileName = testCaseList.at(i)->FullFileName;
        if(fullFileName.compare(tc->FullFileName, Qt::CaseInsensitive) == 0)
        {
            return NULL;
        }
    }

    //Load test-case
    tc->Checked = true;
    testCaseList.append(tc);

    // Load folder cache
    TestCaseFolder *testCaseStatusFolders = DBManager::GetTestCaseCache(tc->FullFileName);

    //Replace STATUS to test-case name
    testCaseStatusFolders->Checked = true;
    testCaseStatusFolders->Expanded = false;
    testCaseStatusFolders->Name = tc->Name;
    testCaseStatusFolders->ownerTestCase = tc;
    setTestCaseOwner(testCaseStatusFolders, tc);

    for(int j = 0; j < testCaseStatusFolders->TestList.count(); j++)
    {
        TestCaseItem *item = testCaseStatusFolders->TestList.at(j);
        item->Checked = true;
        item->status = DBManager::GetTestStatus(tc->FullFileName, item->RelativeFileName);
        item->ownerTestCase = tc;
    }

    MainWindowModel::tree.append(testCaseStatusFolders);

    return testCaseStatusFolders;
}

void MainWindowModel::setTestCaseOwner(TestCaseFolder *folder, TestCase *tc)
{
    folder->Expanded = false;
    folder->ownerTestCase = tc;
    for(int i = 0; i < folder->SubFolderList.length(); i++)
    {
        setTestCaseOwner(folder->SubFolderList.at(i), tc);
    }
}

void MainWindowModel::UpdateRunDescriptions(TestCase *tc)
{
    QVector<int> *runList = DBManager::GetRunList(tc->FullFileName);
    QMap<int, RunDescription *> *runDescriptions = &(tc->RunDescriptions);
    if(runList->length() > runDescriptions->keys().length())
    {
        for(int i = runDescriptions->keys().length(); i < runList->length(); i++)
        {
            RunDescription *runDesc =
                DBManager::GetRunDescription(tc->FullFileName,
                                             DBManager::GetRunName(runList->at(i)));

            runDesc->Checked = true;
            runDescriptions->insert(runDesc->Num, runDesc);
        }
    }
    delete runList;
}

void MainWindowModel::LoadFolderResults(TestCaseFolder *folder)
{
    if(folder == NULL) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == NULL) { return; }

    for(int i = 0; i < folder->TestList.length(); i++)
    {
        TestCaseItem *test = folder->TestList.at(i);

        for(int k = 0; k < tc->RunDescriptions.count(); k++)
        {
            int keyRun = tc->RunDescriptions.keys().at(k);
            if(tc->RunDescriptions.value(keyRun)->Checked)
            {
                TestResult *r = DBManager::GetTestResult(tc->FullFileName,
                                                         test->RelativeFileName,
                                                         DBManager::GetRunName(keyRun));
                test->results.insert(keyRun, r);
            }
        }
    }
}

void MainWindowModel::LoadRunResults(TestCaseFolder *folder, RunDescription *run)
{
    if(run == NULL) { return; }

    if(folder == NULL) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == NULL) { return; }

    //Looking for runKey
    int runNum = tc->RunDescriptions.values().indexOf(run);
    if(runNum < 0) { return; } else { runNum++; }

    for(int i = 0; i < folder->TestList.length(); i++)
    {
        TestCaseItem *test = folder->TestList.at(i);

        TestResult *r = DBManager::GetTestResult(tc->FullFileName,
                                                 test->RelativeFileName,
                                                 DBManager::GetRunName(runNum));
        test->results.insert(runNum, r);
    }
}

void MainWindowModel::LoadTestResults(TestCaseFolder *folder, TestCaseItem *test)
{
    if(test == NULL) { return; }

    if(folder == NULL) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == NULL) { return; }

    for(int k = 0; k < tc->RunDescriptions.count(); k++)
    {
        int keyRun = tc->RunDescriptions.keys().at(k);
        TestResult *r = DBManager::GetTestResult(tc->FullFileName,
                                                 test->RelativeFileName,
                                                 DBManager::GetRunName(keyRun));
        test->results.insert(keyRun, r);
    }
}

void MainWindowModel::LoadOneResult(TestCaseFolder *folder, TestCaseItem *test, RunDescription *run)
{
    if(run == NULL) { return; }

    if(test == NULL) { return; }

    if(folder == NULL) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == NULL) { return; }

    //Looking for runKey
    int runNum = tc->RunDescriptions.values().indexOf(run);
    if(runNum < 0) { return; } else { runNum++; }

    TestResult *r = DBManager::GetTestResult(tc->FullFileName,
                                             test->RelativeFileName,
                                             DBManager::GetRunName(runNum));
    test->results.insert(runNum, r);
}
