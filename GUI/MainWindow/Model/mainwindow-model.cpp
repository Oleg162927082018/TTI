#include "mainwindow-model.h"

#include "Common/DBManager/dbmanager.h"

#include <QFileInfo>

QList<MainWindowTreeFolder *> MainWindowModel::tree;
TestTreeAdapter MainWindowModel::testTreeAdapter;
TestTableAdapter MainWindowModel::testTableAdapter;

MainWindowModel::MainWindowModel()
{

}

void MainWindowModel::AddRunHeader(MainWindowTreeFolder *treeFolder, RunDescription *runDesc, bool isVisible)
{
    MainWindowTableHeader *tHeader = new MainWindowTableHeader();
    tHeader->name = getTableHeader(runDesc);
    tHeader->runDescription = runDesc;

    treeFolder->fullTableHeaders->insert(tHeader->runDescription->Num, tHeader);
    if(isVisible) { treeFolder->visibleTableHeaders->insert(tHeader->runDescription->Num, tHeader); }
}

void MainWindowModel::AddTableItem(MainWindowTreeFolder *treeFolder, MainWindowTableItem *tableItem)
{
    if(treeFolder != nullptr)
    {
        treeFolder->fullTableItems.append(tableItem);
        treeFolder->visibleTableItems.append(tableItem);

        AddTableItem(treeFolder->parentFolder, tableItem);
    }
}

void MainWindowModel::ClearVisibleTableItems(MainWindowTreeFolder *treeFolder)
{
    if(treeFolder != nullptr)
    {
        treeFolder->visibleTableItems.clear();
        foreach (MainWindowTreeFolder *subFolder, treeFolder->subFolders) {
            ClearVisibleTableItems(subFolder);
        }
    }
}

void MainWindowModel::SetVisibleTableItem(MainWindowTreeFolder *treeFolder, MainWindowTableItem *tableItem, bool isVisible)
{
    if(treeFolder != nullptr)
    {
        if(treeFolder->fullTableItems.contains(tableItem))
        {

            if(treeFolder->visibleTableItems.contains(tableItem)) {
                if(!isVisible) { treeFolder->visibleTableItems.removeOne(tableItem); }
            } else {
                if(isVisible) { treeFolder->visibleTableItems.append(tableItem); }
            }

            foreach (MainWindowTreeFolder *subFolder, treeFolder->subFolders) {
                SetVisibleTableItem(subFolder, tableItem, isVisible);
            }
        }
    }
}

void MainWindowModel::setCheckStateSubFolder(MainWindowTreeFolder *folder, int checkState)
{
    folder->checkState = checkState;
    foreach (MainWindowTreeFolder *subfolder, folder->subFolders) {
        setCheckStateSubFolder(subfolder, checkState);
    }}

void MainWindowModel::setParentCheckState(MainWindowTreeFolder *folder)
{
    if(folder != nullptr) {

        int checked_count = 0, unchecked_count = 0, partially_count = 0;

        foreach(MainWindowTreeFolder *subFolder, folder->subFolders) {
            switch(subFolder->checkState) {
                case Qt::Checked: checked_count++;
                    break;
                case Qt::Unchecked: unchecked_count++;
                    break;
                case Qt::PartiallyChecked: partially_count++;
                    break;
            }
        }
        foreach(MainWindowTableItem *tableItem, folder->fullTableItems) {
            if(tableItem->checked) { checked_count++; } else { unchecked_count++; }
        }

        int parentCheckState;
        if((partially_count == 0) && (unchecked_count == 0)) { parentCheckState = Qt::Checked; }
        else if((partially_count == 0) && (checked_count == 0)) { parentCheckState = Qt::Unchecked; }
        else { parentCheckState = Qt::PartiallyChecked; }

        if(folder->checkState != parentCheckState) {
            folder->checkState = parentCheckState;
            setParentCheckState(folder->parentFolder);
        }
    }
}

void MainWindowModel::setCheckState(MainWindowTreeFolder *folder)
{
    bool isChecked = false;

    foreach (MainWindowTableItem *tableItem, folder->fullTableItems) {
        if(!tableItem->checked) {
            isChecked = true;
            break;
        }
    }

    foreach (MainWindowTableItem *tableItem, folder->fullTableItems) {
        tableItem->checked = isChecked;
    }
    setCheckStateSubFolder(folder, isChecked?Qt::Checked:Qt::Unchecked);
    setParentCheckState(folder->parentFolder);

    testTreeAdapter.emitDataChanged();
    testTableAdapter.emitDataChanged();
}

MainWindowTreeFolder *MainWindowModel::findOwnerFolder(MainWindowTableItem *tableItem, MainWindowTreeFolder *rootFolder)
{
    foreach(MainWindowTreeFolder *subFolder, rootFolder->subFolders) {
        if(subFolder->fullTableItems.contains(tableItem)) {
            return findOwnerFolder(tableItem, subFolder);
        }
    }

    return rootFolder;
}

void MainWindowModel::setCheckState(MainWindowTableItem *tableItem, bool isChecked)
{
    tableItem->checked = isChecked;
    foreach (MainWindowTreeFolder *rootFolder, tree) {
        if(rootFolder->ownerTestCase == tableItem->ownerTestCase) {
            MainWindowTreeFolder *ownerFolder = findOwnerFolder(tableItem, rootFolder);
            setParentCheckState(ownerFolder);
            testTreeAdapter.emitDataChanged();
            testTableAdapter.emitDataChanged();
            break;
        }
    }
}

void MainWindowModel::LoadTreeFolder(MainWindowTreeFolder *treeFolder, QString relativeTreeFolderName)
{
    QStringList subFolders = DBManager::GetTestCaseFolders(treeFolder->ownerTestCase->fullFileName, relativeTreeFolderName);
    QList<TestStatus *> subItems = DBManager::GetTestCaseFolderItems(treeFolder->ownerTestCase->fullFileName, relativeTreeFolderName);

    for(int i = 0; i < subFolders.count(); i++)
    {
        MainWindowTreeFolder *subTreeFolder = new MainWindowTreeFolder();
        subTreeFolder->name = subFolders.at(i);
        subTreeFolder->parentFolder = treeFolder;
        subTreeFolder->ownerTestCase = treeFolder->ownerTestCase;
        subTreeFolder->fullTableHeaders = treeFolder->fullTableHeaders;
        subTreeFolder->visibleTableHeaders = treeFolder->visibleTableHeaders;

        QString subFolder = relativeTreeFolderName;
        if(!subFolder.isEmpty()) { subFolder += "/"; }
        subFolder += subFolders.at(i);
        LoadTreeFolder(subTreeFolder, subFolder);

        treeFolder->subFolders.append(subTreeFolder);
    }

    for(int i = 0; i < subItems.count(); i++)
    {
        MainWindowTableItem *tableItem = new MainWindowTableItem();
        TestStatus *status = subItems.at(i);
        tableItem->status = status;
        tableItem->name = status->relativeFileName.mid(status->relativeFileName.lastIndexOf("/") + 1);
        tableItem->ownerTestCase = treeFolder->ownerTestCase;
        tableItem->fullTableHeaders = treeFolder->fullTableHeaders;
        tableItem->visibleTableHeaders = treeFolder->visibleTableHeaders;

        AddTableItem(treeFolder, tableItem);
    }

}

MainWindowTreeFolder *MainWindowModel::LoadTestCase(TestCase *tc)
{
    //Check test-case already open
    for(int i = 0; i < tree.count(); i++)
    {
        QString fullFileName = tree.at(i)->ownerTestCase->fullFileName;
        if(fullFileName.compare(tc->fullFileName, Qt::CaseInsensitive) == 0)
        {
            return nullptr;
        }
    }

    //Load test-case
    MainWindowTreeFolder *testCaseFolder = new MainWindowTreeFolder();
    testCaseFolder->name = tc->fullFileName.mid(tc->fullFileName.lastIndexOf("/") + 1);
    testCaseFolder->ownerTestCase = tc;
    testCaseFolder->fullTableHeaders = new QMap<int, MainWindowTableHeader *>();
    testCaseFolder->visibleTableHeaders = new QMap<int, MainWindowTableHeader *>();

    LoadTreeFolder(testCaseFolder,"");

    MainWindowModel::tree.append(testCaseFolder);
    return testCaseFolder;
}

void MainWindowModel::setTestCaseOwner(MainWindowTreeFolder *folder, TestCase *tc)
{
    folder->expanded = false;
    folder->ownerTestCase = tc;
    for(int i = 0; i < folder->subFolders.length(); i++)
    {
        setTestCaseOwner(folder->subFolders.at(i), tc);
    }
}

void MainWindowModel::UpdateRunDescriptions(MainWindowTreeFolder *testCaseFolder)
{
    QString testCaseFullFileName = testCaseFolder->ownerTestCase->fullFileName;
    QVector<int> *runList = DBManager::GetRunList(testCaseFullFileName);

    for(int i = 0; i < runList->count(); i++)
    {
        int runNum = runList->at(i);
        if(!testCaseFolder->fullTableHeaders->keys().contains(runNum)) {

            RunDescription *rd = DBManager::GetRunDescription(testCaseFullFileName, DBManager::GetRunName(runNum));

            MainWindowTableHeader *th = new MainWindowTableHeader();
            th->name = getTableHeader(rd);
            th->runDescription = rd;
            testCaseFolder->fullTableHeaders->insert(runNum, th);
        }
    }
    delete runList;
}

void MainWindowModel::LoadFolderResults(MainWindowTreeFolder *folder)
{
    if(folder == nullptr) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == nullptr) { return; }

    for(int i = 0; i < folder->visibleTableItems.length(); i++)
    {
        MainWindowTableItem *test = folder->visibleTableItems.at(i);

        for(int k = 0; k < folder->visibleTableHeaders->count(); k++)
        {
            int keyRun = folder->visibleTableHeaders->keys().at(k);
            TestResult *r = DBManager::GetTestResult(tc->fullFileName,
                                                     test->status->relativeFileName,
                                                     DBManager::GetRunName(keyRun));
            test->results.insert(keyRun, r);
        }
    }
}

void MainWindowModel::LoadRunResults(MainWindowTreeFolder *folder, MainWindowTableHeader *run)
{
    if(run == nullptr) { return; }

    if(folder == nullptr) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == nullptr) { return; }

    for(int i = 0; i < folder->visibleTableItems.count(); i++)
    {
        MainWindowTableItem *test = folder->visibleTableItems.at(i);
        TestResult *r = DBManager::GetTestResult(tc->fullFileName,
                                                 test->status->relativeFileName,
                                                 DBManager::GetRunName(run->runDescription->Num));

        test->results.insert(run->runDescription->Num, r);
    }
}

void MainWindowModel::LoadTestResults(MainWindowTreeFolder *folder, MainWindowTableItem *test)
{
    if(test == nullptr) { return; }
    if(folder == nullptr) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == nullptr) { return; }

    for(int k = 0; k < folder->visibleTableHeaders->count(); k++)
    {
        int runNum = folder->visibleTableHeaders->keys().at(k);
        TestResult *r = DBManager::GetTestResult(tc->fullFileName,
                                                 test->status->relativeFileName,
                                                 DBManager::GetRunName(runNum));
        test->results.insert(runNum, r);
    }
}

void MainWindowModel::LoadOneResult(MainWindowTreeFolder *folder, MainWindowTableItem *test, MainWindowTableHeader *run)
{
    if(run == nullptr) { return; }
    if(test == nullptr) { return; }
    if(folder == nullptr) { return; }

    TestCase *tc = folder->ownerTestCase;

    if(tc == nullptr) { return; }

    TestResult *r = DBManager::GetTestResult(tc->fullFileName,
                                             test->status->relativeFileName,
                                             DBManager::GetRunName(run->runDescription->Num));
    test->results.insert(run->runDescription->Num, r);
}

void MainWindowModel::DeleteTree()
{
    for(int i = 0; i < tree.count(); i++)
    {
        MainWindowTreeFolder *treeFolder = tree.at(i);

        delete treeFolder->ownerTestCase;

        for(int j = 0; j < treeFolder->fullTableHeaders->count(); j++)
        {
            MainWindowTableHeader *tHeader = treeFolder->fullTableHeaders->values().at(j);
            delete tHeader;
        }
        delete treeFolder->fullTableHeaders;
        delete treeFolder->visibleTableHeaders;

        for(int j = 0; j < treeFolder->fullTableItems.count(); j++)
        {
            MainWindowTableItem *tableItem = treeFolder->fullTableItems.at(j);
            delete tableItem->status;
            for(int k = 0; k < tableItem->results.count(); k++)
            {
                delete tableItem->results.values().at(k);
            }
        }

        DeleteTreeFolder(treeFolder);
    }
}

void MainWindowModel::DeleteTreeFolder(MainWindowTreeFolder *treeFolder)
{
    for(int i = 0; i < treeFolder->subFolders.count(); i++)
    {
        DeleteTreeFolder(treeFolder->subFolders.at(i));
    }

    delete treeFolder;
}

QString MainWindowModel::getTableHeader(RunDescription *rd)
{
    return rd->LocalDateTimeOfStart.toString("dd-MMM-yyyy HH:mm:ss");
}
