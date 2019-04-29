#include "open-testcase-model.h"

#include <Common/DBManager/dbmanager.h>
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
        OpenTestCaseItem *tcItem = new OpenTestCaseItem();
        tcItem->isLoadResultsImediately = false;
        tcItem->name = tcInf.fileName();
        tcItem->testCase = DBManager::GetTestCase(tcInf.filePath());

        tcItem->isDescriptionFilter = false;
        tcItem->descriptionFilter = "";
        tcItem->isFromDateTimeFilter = false;
        tcItem->isToDateTimeFilter = false;

        //Load run descriptions
        QVector<int> *runIdList = DBManager::GetRunList(tcItem->testCase->fullFileName);
        for(int i = 0; i < runIdList->length(); i++)
        {
            int runNum = runIdList->at(i);
            OpenTestCaseRunItem *runItem = new OpenTestCaseRunItem();

            runItem->checked = true;
            runItem->runDescription = DBManager::GetRunDescription(tcInf.filePath(), DBManager::GetRunName(runNum));
            runItem->name = runItem->runDescription->LocalDateTimeOfStart.toString("dd-MMM-yyyy HH:mm:ss");
            if(tcItem->fromDateTimeFilter.isNull() || tcItem->toDateTimeFilter.isNull()) {
                tcItem->fromDateTimeFilter = tcItem->toDateTimeFilter = runItem->runDescription->LocalDateTimeOfStart;
            } else {
                if(tcItem->fromDateTimeFilter > runItem->runDescription->LocalDateTimeOfStart)
                    { tcItem->fromDateTimeFilter = runItem->runDescription->LocalDateTimeOfStart; }
                if(tcItem->toDateTimeFilter < runItem->runDescription->LocalDateTimeOfStart)
                    { tcItem->toDateTimeFilter = runItem->runDescription->LocalDateTimeOfStart; }
            }

            tcItem->fullRunDescriptions.insert(runItem->runDescription->Num, runItem);
            tcItem->visibleRunDescriptions.insert(runItem->runDescription->Num, runItem);
        }
        delete runIdList;

        if(tcItem->fromDateTimeFilter.isNull() || tcItem->toDateTimeFilter.isNull()) {
            tcItem->fromDateTimeFilter = tcItem->toDateTimeFilter = QDateTime::fromTime_t(0);
        }

        itemList.append(tcItem);
    }
}

void OpenTestCaseModel::removeTestCase(int ind)
{
    OpenTestCaseItem *tcItem = itemList.at(ind);
    for(int i = 0; i < tcItem->fullRunDescriptions.count(); i++)
    {
        OpenTestCaseRunItem *runItem = tcItem->fullRunDescriptions.values().at(i);
        delete runItem->runDescription;
        delete runItem;
    }

    itemList.removeAt(ind);
    delete tcItem;
}

void OpenTestCaseModel::clear()
{
    while(itemList.length() > 0)
    {
        removeTestCase(0);
    }
}

bool OpenTestCaseModel::isTestCaseContained(QString testCaseFileName)
{
    for(int i = 0; i < itemList.length(); i++)
    {
        if(itemList.at(i)->testCase->fullFileName.compare(testCaseFileName, Qt::CaseInsensitive) == 0)
        {
            return true;
        }
    }
    return false;
}
