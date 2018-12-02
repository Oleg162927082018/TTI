#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Common/DllManager/dllmanager.h>

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <GUI/TestCase/Create/View/create-testcase-dialog.h>
#include <GUI/MainWindow/Model/mainwindow-model.h>
#include <GUI/TestCase/Open/View/open-testcase-dialog.h>
#include <GUI/Run/PlanDispatcher/View/plan-dispatcher-dialog.h>
#include <HelpManager/helpmanager.h>
#include <GUI/Search/View/set-filter-dialog.h>
#include <GUI/RunHeader/RunHeadersDialog/View/run-headers-dialog.h>
#include <GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.h>

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::saveTreeState()
{
    bakupTreeIndex = ui->testTreeView->currentIndex();

    if(!MainWindowModel::tree.isEmpty())
    {
        for(int i = 0; i < MainWindowModel::tree.length(); i++)
        {
            QModelIndex rootIndex = testTreeAdapter.index(i, 0);
            saveExpandedState(rootIndex);
        }
    }
}

void MainWindow::loadTreeState()
{
    if(!MainWindowModel::tree.isEmpty())
    {
        for(int i = 0; i < MainWindowModel::tree.length(); i++)
        {
            QModelIndex rootIndex = testTreeAdapter.index(i,0);
            loadExpandedState(rootIndex);
        }

        if(bakupTreeIndex.isValid())
        {
            ui->testTreeView->setCurrentIndex(bakupTreeIndex);
        }
        else
        {
            ui->testTreeView->setCurrentIndex(testTreeAdapter.index(0,0));
        }
    }

}

void MainWindow::saveExpandedState(QModelIndex &index)
{
    MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
    folder->expanded = ui->testTreeView->isExpanded(index);

    for(int i = 0; i < folder->subFolders.length(); i++)
    {
        QModelIndex subIndex = testTreeAdapter.index(i, 0, index);
        saveExpandedState(subIndex);
    }
}

void MainWindow::loadExpandedState(QModelIndex &index)
{
    MainWindowTreeFolder* folder = static_cast<MainWindowTreeFolder*>(index.internalPointer());
    ui->testTreeView->setExpanded(index, folder->expanded);

    for(int i = 0; i < folder->subFolders.length(); i++)
    {
        QModelIndex subIndex = testTreeAdapter.index(i, 0, index);
        loadExpandedState(subIndex);
    }
}

void MainWindow::on_actionNewTestCase_triggered()
{
    QStringList IDs = DLLManager::GetTestCaseAdapterIDList();

    if(IDs.count() == 0)
    {
        QMessageBox mb("Error",
                       "You have not any templates for creating test-case!",
                       QMessageBox::Critical,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();

        return;
    }

    //Create default folder for save test cases
    QDir appFolder(qApp->applicationDirPath());
    appFolder.mkdir("tests");

    //Get file name for test case
    QString testCaseFileName = QFileDialog::getSaveFileName(this, "New Test case",
                               appFolder.filePath("tests"), "Test case (*.tcs)");

    if(!testCaseFileName.isEmpty()&& !testCaseFileName.isNull()){
        //Input params of test case
        CreateTestCaseDialog createTestCaseDialog(testCaseFileName, IDs, this);
        if(createTestCaseDialog.exec() == QDialog::Accepted)
        {
            TestCase *tc = createTestCaseDialog.GetTestCase();

            //Open new test-case
            saveTreeState();
            testTreeAdapter.beginResetModel();

            MainWindowModel::LoadTestCase(tc);

            testTreeAdapter.endResetModel();
            loadTreeState();
        }
    }
}

void MainWindow::on_actionOpenTestCase_triggered()
{
    OpenTestCaseDialog openTestCaseDialog(this);
    if(openTestCaseDialog.exec() == QDialog::Accepted)
    {
        QList<OpenTestCaseItem *> openTestCaseItems = openTestCaseDialog.GetOpenTestCaseItemList();

        saveTreeState();
        testTreeAdapter.beginResetModel();
        testTableAdapter.beginResetModel();

        for(int i = 0; i < openTestCaseItems.count(); i++)
        {
            OpenTestCaseItem *openTCItem = openTestCaseItems.at(i);

            bool isLoadResult = openTCItem->isLoadResultsImediately;
            MainWindowTreeFolder *tcf = MainWindowModel::LoadTestCase(openTCItem->testCase);
            for(int j = 0; j < openTCItem->fullRunDescriptions.count(); j++)
            {
                OpenTestCaseRunItem *openTCRunItem = openTCItem->fullRunDescriptions.values().at(j);
                MainWindowModel::AddRunHeader(tcf, openTCRunItem->runDescription, openTCRunItem->checked);
                delete openTCRunItem;
            }
            delete openTCItem;

            if((tcf != nullptr) && isLoadResult) {
                //Load results
                MainWindowModel::LoadFolderResults(tcf);
            } else if(tcf == nullptr) {
                //Test-case already open
            }
        }

        testTableAdapter.endResetModel();
        testTreeAdapter.endResetModel();
        loadTreeState();
    }
}

void MainWindow::on_actionShow_RunHeader_triggered()
{
    //Get table item
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder *>(indexTree.internalPointer());
    if(tf == nullptr) { return; }

    MainWindowModel::UpdateRunDescriptions(tf);

    RunHeadersDialog runDescriptionShowDialog(this);
    for(int i = 0; i < tf->fullTableHeaders->count(); i++) {
        MainWindowTableHeader *h = tf->fullTableHeaders->values().at(i);
        runDescriptionShowDialog.addRunDescription(
                    h->name, tf->visibleTableHeaders->values().contains(h), h->runDescription);
    }

    runDescriptionShowDialog.exec();

    testTableAdapter.beginResetModel();

    QMap<int, RunHeaderItem *> *runHeaders = runDescriptionShowDialog.getRunDescriptions();
    for(int i = 0; i < runHeaders->count(); i++) {

        RunHeaderItem *h = runHeaders->values().at(i);

        if(!tf->fullTableHeaders->keys().contains(h->rd->Num)) {

            MainWindowModel::AddRunHeader(tf, h->rd, h->checked);

        } else if((h->checked) && (!tf->visibleTableHeaders->keys().contains(h->rd->Num))) {

            tf->visibleTableHeaders->insert(h->rd->Num, tf->fullTableHeaders->value(h->rd->Num));

        } else if((!h->checked) && (tf->visibleTableHeaders->keys().contains(h->rd->Num))) {

            tf->visibleTableHeaders->remove(h->rd->Num);

        }
    }

    testTableAdapter.endResetModel();
}

void MainWindow::on_actionRun_dispatcher_triggered()
{
    PlanDispatcherDialog dispatcherTestPlanDialog(this);
    dispatcherTestPlanDialog.exec();
}

void MainWindow::UpdateTestStatus(int runNum, QString newStatus, bool checked)
{
    //Get table item
    QModelIndex tableIndex = ui->testTableView->currentIndex();
    MainWindowTableItem *testItem = testTableAdapter.getRowData(tableIndex.row());
    if(testItem == nullptr) { return; }

    QMap<int, BenchmarkInfo> &currentTestBenchmarks = testItem->status->benchmarks;

    if(checked)
    {
        //Try to update benchmark status if benchmark is present in the list
        if(currentTestBenchmarks.contains(runNum))
        {
            if(currentTestBenchmarks.value(runNum).status.compare(newStatus) == 0) { return; }

            QModelIndex tableInd = ui->testTableView->currentIndex();
            testTableAdapter.beginResetModel();

            currentTestBenchmarks[runNum].status = newStatus;

            testTableAdapter.endResetModel();
            ui->testTableView->setCurrentIndex(tableInd);
        }
        else
        {
            //Benchmark not present, need to insert new benchmark
            BenchmarkInfo benchmarkInfo;

            benchmarkInfo.outMark = testItem->results.value(runNum)->outMark;
            benchmarkInfo.status = newStatus;

            QModelIndex tableInd = ui->testTableView->currentIndex();
            testTableAdapter.beginResetModel();

            currentTestBenchmarks.insert(runNum, benchmarkInfo);

            testTableAdapter.endResetModel();
            ui->testTableView->setCurrentIndex(tableInd);
        }
    }
    else
    {
        if(!currentTestBenchmarks.contains(runNum)) { return; }

        //Need delete benchmark
        QModelIndex tableInd = ui->testTableView->currentIndex();
        testTableAdapter.beginResetModel();

        currentTestBenchmarks.remove(runNum);

        testTableAdapter.endResetModel();
        ui->testTableView->setCurrentIndex(tableInd);
    }

    DBManager::SaveTestStatus(testItem->ownerTestCase->fullFileName, testItem->status);
}

void MainWindow::on_actionLoadAllResults_triggered()
{
    QModelIndex indexTable = ui->testTableView->currentIndex();

    testTableAdapter.beginResetModel();

    for(int i = 0; i < MainWindowModel::tree.length(); i++)
    {
        MainWindowModel::LoadFolderResults(MainWindowModel::tree.at(i));
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadTestCaseResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder *>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    testTableAdapter.beginResetModel();

    if(tf != nullptr)
    {
        while(tf->parentFolder != nullptr) { tf = tf->parentFolder; }
        MainWindowModel::LoadFolderResults(tf);
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadFolderResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder *>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    testTableAdapter.beginResetModel();

    if(tf != nullptr)
    {
        MainWindowModel::LoadFolderResults(tf);
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadTestResult_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder *>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();
    MainWindowTableItem *test = tf->visibleTableItems.at(indexTable.row());

    testTableAdapter.beginResetModel();

    if((tf != nullptr) && (test != nullptr))
    {
        MainWindowModel::LoadTestResults(tf, test);
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadRunResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder *>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    int colRun = indexTable.column() - 1;
    if(colRun < 0) { return; }
    int keyRun = tf->visibleTableHeaders->keys().at(colRun);
    MainWindowTableHeader *run = tf->visibleTableHeaders->value(keyRun);

    testTableAdapter.beginResetModel();

    if((tf != nullptr) && (run != nullptr))
    {
        MainWindowModel::LoadRunResults(tf, run);
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadOneResult_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    MainWindowTreeFolder *tf = static_cast<MainWindowTreeFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();
    MainWindowTableItem *test = tf->visibleTableItems.at(indexTable.row());

    int colRun = indexTable.column() - 1;
    if(colRun < 0) { return; }
    int keyRun = tf->visibleTableHeaders->keys().at(colRun);
    MainWindowTableHeader *run = tf->visibleTableHeaders->value(keyRun);

    testTableAdapter.beginResetModel();

    if((tf != nullptr) && (test != nullptr))
    {
        MainWindowModel::LoadOneResult(tf, test, run);
    }

    testTableAdapter.endResetModel();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::AttachTag()
{
    TagDispatcherDialog dlg(TagDispatcherDialog::ACTION_SELECT, this);
    dlg.exec();

    TagItem *tagItem = dlg.getSelectResult();
    if((tagItem != nullptr) && (tagItem->tag != nullptr))
    {

        //Get table item
        QModelIndex tableIndex = ui->testTableView->currentIndex();
        MainWindowTableItem *testItem = testTableAdapter.getRowData(tableIndex.row());
        if(testItem == nullptr) { return; }

        QModelIndex tableInd = ui->testTableView->currentIndex();
        testTableAdapter.beginResetModel();

        testItem->status->tags.append(tagItem->path);
        DBManager::SaveTestStatus(testItem->ownerTestCase->fullFileName, testItem->status);

        testTableAdapter.endResetModel();
        ui->testTableView->setCurrentIndex(tableInd);
    }
}

void MainWindow::on_actionTagCollections_triggered()
{
    QStringList IDs = TagManager::GetTagAdapterIDList();
    if(IDs.count() == 0)
    {
        QMessageBox mb("Error",
                       "You have not any tag collections",
                       QMessageBox::Critical,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();
        return;
    }

    TagDispatcherDialog dlg(TagDispatcherDialog::ACTION_EDIT, this);
    dlg.exec();
}

void MainWindow::on_actionAttachTag_triggered()
{
    AttachTag();
}

void MainWindow::on_actionContentsHelp_triggered()
{
    HelpManager::showContents();
}

void MainWindow::on_actionIndexHelp_triggered()
{
    HelpManager::showIndex();
}

void MainWindow::on_actionSet_Filter_triggered()
{
    SetFilterDialog dlg(this);
    dlg.exec();
}

