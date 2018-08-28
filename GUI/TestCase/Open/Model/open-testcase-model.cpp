#include "open-testcase-model.h"

#include <QFileInfo>

OpenTestCaseModel::OpenTestCaseModel()
{

}

void OpenTestCaseModel::addTestCase(QString testCaseFileName)
{
    QFileInfo tcInf(testCaseFileName);
    if(tcInf.exists())
    {
        //Load Test-Case
        TestCase *tc = DBManager::GetTestCase(tcInf.filePath());
        tc->Checked = false; //is load results immediately
        tc->Name = tcInf.fileName();
        tc->FullFileName = tcInf.filePath();

        //Load all run descriptions
        QVector<int> *runList = DBManager::GetRunList(tc->FullFileName);
        for(int i = 0; i < runList->length(); i++)
        {
            int runNum = runList->at(i);
            RunDescription *desc = DBManager::GetRunDescription(tcInf.filePath(), DBManager::GetRunName(runNum));
            desc->Checked = false;
            tc->RunDescriptions.insert(desc->Num, desc);
        }
        delete runList;

        testCaseList.append(tc);
    }
}

void OpenTestCaseModel::removeTestCase(int ind)
{
    TestCase *tc = testCaseList.at(ind);
    if(currentTestCase == tc)
    {
        currentTestCase = NULL;
    }

    for(int i = 0; i < tc->RunDescriptions.keys().length(); i++)
    {
        int key = tc->RunDescriptions.keys().at(i);
        RunDescription *rd = tc->RunDescriptions.value(key);
        delete rd;
    }

    testCaseList.removeAt(ind);
    delete tc;
}

void OpenTestCaseModel::clear()
{
    while(testCaseList.length() > 0)
    {
        removeTestCase(0);
    }
}

bool OpenTestCaseModel::isTestCaseContained(QString testCaseFileName)
{
    for(int i = 0; i < testCaseList.length(); i++)
    {
        if(testCaseList.at(i)->FullFileName.compare(testCaseFileName, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }
    return false;
}
