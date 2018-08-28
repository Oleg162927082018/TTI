#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Common/DllManager/dllmanager.h"
#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

#include "GUI/MainWindow/Model/mainwindow-model.h"
#include "GUI/Run/PlanDispatcher/plan-dispatcher-dialog.h"
#include "GUI/TestCase/Create/create-testcase-dialog.h"
#include "GUI/TestCase/Open/View/open-testcase-dialog.h"
#include "GUI/Run/RunDescription-Show/View/rundescription-show-dialog.h"
#include "GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.h"
#include "GUI/Tag/View/View/tag-view-dialog.h"

#include "TagManager/tagmanager.h"
#include "HelpManager/helpmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QtHelp/QHelpEngineCore>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    testTableAdapter.Init(NULL, NULL);

    ui->testTreeView->setModel(&testTreeAdapter);
    ui->testTableView->setModel(&testTableAdapter);

    connect(ui->testTreeView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_testTreeViewSelectionChanged(QItemSelection,QItemSelection)));

    connect(ui->testTableView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_testTableViewSelectionChanged(QItemSelection,QItemSelection)));

    DLLManager::Init();
    RunManager::Init();
    TagManager::Init();
    HelpManager::Init();

    QStringList testCaseTemplateIDs = DLLManager::GetTestCaseAdapterIDList();
    for(int i = 0; i < testCaseTemplateIDs.count(); i++)
    {
        QString key = testCaseTemplateIDs.at(i);
        IResultCompareWidget *compareWidget = DLLManager::GetTestCaseResultCompareWidget(key);
        compareWidgets.insert(key, compareWidget);
        if(compareWidget != NULL)
        {
            ui->extraCompareWidget->addWidget(compareWidget);
        }
    }

    // Table context menu
    QAction *cellLoadAction = new QAction("load Result", this);
    tableContextMenu.addAction(cellLoadAction);
    connect(cellLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadOneResult_triggered()));

    QAction *testLoadAction = new QAction("load Results for test", this);
    tableContextMenu.addAction(testLoadAction);
    connect(testLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadTestResult_triggered()));

    QAction *runLoadAction = new QAction("load Results for Run", this);
    tableContextMenu.addAction(runLoadAction);
    connect(runLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadRunResults_triggered()));

    QAction *allLoadAction = new QAction("load Results for Folder", this);
    tableContextMenu.addAction(allLoadAction);
    connect(allLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadFolderResults_triggered()));


    //Table head context menu
    ui->testTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->testTableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(on_testTableHeaderView_customContextMenuRequested(QPoint)));

    QAction *hideRunAction = new QAction("Hide Run", this);
    tableHeadContextMenu.addAction(hideRunAction);
    //connect(hideRunAction, SIGNAL(triggered()),
    //      this, SLOT(on_actionLoadFolderResults_triggered()));

    QAction *showRunAction = new QAction("Show Runs", this);
    tableHeadContextMenu.addAction(showRunAction);
    connect(showRunAction, SIGNAL(triggered()),
          this, SLOT(on_actionShowRun_triggered()));

    QAction *descRunAction = new QAction("edit Run Description", this);
    tableHeadContextMenu.addAction(descRunAction);
    //connect(descRunAction, SIGNAL(triggered()),
    //      this, SLOT(on_actionLoadFolderResults_triggered()));

    // Tree context menu
    QAction *folderLoadAction = new QAction("Load", this);
    treeContextMenu.addAction(folderLoadAction);
    connect(folderLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadFolderResults_triggered()));
}

MainWindow::~MainWindow()
{
    DLLManager::FreeResources();
    RunManager::FreeResources();
    TagManager::FreeResources();
    HelpManager::FreeResources();

    delete ui;
    // Delete data model
    /*for(int i = 0; i < MainWindowModel::testCaseList.count(); i++)
    {
        TestCase *tc = MainWindowModel::testCaseList.at(i);

        for(int j = 0; j < tc->RunDescriptions.count(); j++)
        {
            //delete tc->RunDescriptions.values().at(j);
        }

        //delete tc;
    }*/

    // Because parent folder must contain links to all TestCaseItem in subfolders
    // we must delete items onсe in root folder.

    /*for(int i = 0; i < MainWindowModel::tree.count(); i++)
    {
        TestCaseFolder *rootFolder = MainWindowModel::tree.at(i);

        for(int j = 0; j < rootFolder->TestList.count(); j++)
        {
            TestCaseItem *item = rootFolder->TestList.at(i);

            for(int k = 0; k < item->results.count(); k++)
            {
                //delete item->results.values().at(k);
            }

            //delete item;
        }
    }*/

    // And need delete folders recursively
    for(int i = 0; i < MainWindowModel::tree.count(); i++)
    { FreeTreeModel(MainWindowModel::tree.at(i)); }

    /*for(int i = 0; i < compareWidgets.values().count(); i++)
    {
        IResultCompareWidget *compareWidget = compareWidgets.values().at(i);
        delete compareWidget;
    }*/
}

TestCaseItem *MainWindow::GetCurrentTestItem()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());
    if(tcf == NULL) { return NULL; }

    // Get test item
    QModelIndex tableIndex = ui->testTableView->currentIndex();
    return tcf->TestList.at(tableIndex.row());
}

void MainWindow::FreeTreeModel(TestCaseFolder *folder)
{
    for(int i = 0; i < folder->SubFolderList.count(); i++)
    { FreeTreeModel(folder->SubFolderList.at(i)); }

    //delete folder;
}

void MainWindow::BeginMainWindowUpdate()
{
    isMainWindowUpdating = true;
}

void MainWindow::EndMainWindowUpdate()
{
    isMainWindowUpdating = false;
}

void MainWindow::on_testTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection)
{
    if(!isMainWindowUpdating)
    {
        // Disable event reaction
        BeginMainWindowUpdate();

        TestCaseFolder *currentTestCaseFolder = NULL;
        TestCase *currentTestCase = NULL;
        if(newSelection.length() > 0)
        {
            QModelIndex newFirstIndex = newSelection.at(0).indexes().at(0);
            currentTestCaseFolder = static_cast<TestCaseFolder*>(newFirstIndex.internalPointer());
        }
        if(currentTestCaseFolder == NULL) { return; }
        currentTestCase = currentTestCaseFolder->ownerTestCase;

        TestCaseFolder *oldTestCaseFolder = NULL;
        TestCase *oldTestCase = NULL;
        if(oldSelection.length() > 0)
        {
            QModelIndex oldFirstIndex = oldSelection.at(0).indexes().at(0);
            oldTestCaseFolder = static_cast<TestCaseFolder*>(oldFirstIndex.internalPointer());
            if(oldTestCaseFolder != NULL)
            {
                oldTestCase = oldTestCaseFolder->ownerTestCase;
            }
        }

        // Change table view
        testTableAdapter.beginResetModel();
        testTableAdapter.Init(&(currentTestCaseFolder->TestList), &(currentTestCase->RunDescriptions));

        if(currentTestCase != oldTestCase)
        {
            ui->leftCompareComboBox->clear();
            ui->rightCompareComboBox->clear();

            for(int i = 0; i < currentTestCase->RunDescriptions.keys().length(); i++)
            {
                //Hide Run column
                int key = currentTestCase->RunDescriptions.keys().at(i);
                ui->testTableView->setColumnHidden(i+1, !currentTestCase->RunDescriptions.value(key)->Checked);
                ui->testTableView->setColumnWidth(i+1, 100);

                //Upload Left and Right detail combo boxes
                RunDescription *desc = currentTestCase->RunDescriptions.value(key);
                if(desc != NULL)
                {
                    ui->leftCompareComboBox->addItem(desc->LocalDateTimeOfStart.toString(Qt::SystemLocaleDate));
                    ui->rightCompareComboBox->addItem(desc->LocalDateTimeOfStart.toString(Qt::SystemLocaleDate));
                }
            }
        }

        testTableAdapter.endResetModel();
        // End change table view


        // Select compare widget
        if(currentTestCase != oldTestCase)
        {
            IResultCompareWidget *newCompareWidget = compareWidgets.value(currentTestCase->ID);
            if(newCompareWidget != NULL)
            {
                ui->extraCompareWidget->setCurrentWidget(newCompareWidget);
            }
            else
            {
                 ui->extraCompareWidget->setCurrentIndex(0);
            }
        }

        // Enable event reaction
        EndMainWindowUpdate();

        // Set first element of the table active
        QModelIndex firstTestIndex =
                ui->testTableView->indexAt(QPoint(ui->testTableView->viewport()->rect().x()+1,
                                                  ui->testTableView->viewport()->rect().y()+1));
        ui->testTableView->setCurrentIndex(firstTestIndex);
        ui->testTableView->setColumnWidth(0, 300);

        // And raise event on_testTableViewSelectionChanged
    }
}

void MainWindow::on_testTableViewSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    if(!isMainWindowUpdating)
    {
        // Get test item
        TestCaseItem *testItem = GetCurrentTestItem();
        if(testItem == NULL) { return; }

        BeginMainWindowUpdate();

        FillLeftDetailPanel(testItem);
        FillRightDetailPanel(testItem);
        UpdateTagListForCurrentTest();

        EndMainWindowUpdate();
    }
}

void MainWindow::UpdateTagListForCurrentTest()
{
    TestCaseItem *testItem = GetCurrentTestItem();
    if(testItem == NULL) { return; }

    //Temporary code for displaying tags
    ui->tagBox->clear();

    QString tagList = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\
<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">p, li { white-space: pre-wrap; }</style>\
</head><body style=\"font-family:'MS Shell Dlg 2'; font-size:7.8pt; font-weight:400; font-style:normal;\">\
<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">";

    QString concatenator = "";

    for(int i = 0; i < testItem->status->tags.count(); i++)
    {
        tagList +=  concatenator + "<span style=\" font-size:8.25pt; text-decoration: underline; color:#0000ff;\"><a href=\"" +
                    testItem->status->tags.at(i) + "#view\">" + testItem->status->tags.at(i) + "</a> [<a href=\"" +
                    testItem->status->tags.at(i) + "#remove\">Х</a>]</span>";
        concatenator = "<span style=\" font-size:8.25pt;\">; </span>";
    }
    tagList += "</p></body></html>";

    ui->tagBox->setHtml(tagList);
}

void MainWindow::FillLeftDetailPanel(TestCaseItem *testItem)
{
    // Get run number for the left detail panel
    int leftComboBoxIndex = ui->leftCompareComboBox->currentIndex();
    int leftRunNum = testItem->ownerTestCase->RunDescriptions.keys().at(leftComboBoxIndex);

    // Get widget
    IResultCompareWidget *compareWidget = compareWidgets.value(testItem->ownerTestCase->ID);
    if(compareWidget==NULL) { return; }

    // Get test result
    TestResult *testResult = testItem->results.value(leftRunNum);

    // Clear panel boxes
    ui->leftPerfectBox->setChecked(false);
    ui->leftTheBestBox->setChecked(false);
    ui->leftLogBox->clear();

    // Get outMark
    QString outMark;
    if(testResult != NULL)
    {
        outMark = testResult->outMark;
    }

    // Fill left panel
    if(!outMark.isEmpty())
    {
        //Set log data
        QByteArray logData = DBManager::GetConsoleLog(testItem->ownerTestCase->FullFileName,
                                               testItem->RelativeFileName,
                                               DBManager::GetRunName(leftRunNum));

        ui->leftLogBox->appendPlainText(logData);

        //Set output data
        compareWidget->SetLeftValue(DBManager::GetOutFolder(testItem->ownerTestCase->FullFileName,
                                                            testItem->RelativeFileName,
                                                            outMark));

        if(testItem->status->benchmarks.contains(leftRunNum))
        {
            if(testItem->status->benchmarks.value(leftRunNum).status.compare("perfect") == 0)
            {
                ui->leftPerfectBox->setChecked(true);
            }
            else
            {
                ui->leftTheBestBox->setChecked(true);
            }
        }
    }
    else
    {
        compareWidget->SetLeftValue(QString());
    }
}

void MainWindow::FillRightDetailPanel(TestCaseItem *testItem)
{
    // Get run number for the right detail panel
    int rightComboBoxIndex = ui->rightCompareComboBox->currentIndex();
    int rightRunNum = testItem->ownerTestCase->RunDescriptions.keys().at(rightComboBoxIndex);

    // Get widget
    IResultCompareWidget *compareWidget = compareWidgets.value(testItem->ownerTestCase->ID);
    if(compareWidget==NULL) { return; }

    // Get test result
    TestResult *testResult = testItem->results.value(rightRunNum);

    // Clear panel boxes
    ui->rightPerfectBox->setChecked(false);
    ui->rightTheBestBox->setChecked(false);
    ui->rightLogBox->clear();

    // Get outMark
    QString outMark;
    if(testResult != NULL)
    {
        outMark = testResult->outMark;
    }

    // Fill right panel
    if(!outMark.isEmpty())
    {
        //Set log data
        QByteArray logData = DBManager::GetConsoleLog(testItem->ownerTestCase->FullFileName,
                                               testItem->RelativeFileName,
                                               DBManager::GetRunName(rightRunNum));

        ui->rightLogBox->appendPlainText(logData);

        //Set output data
        compareWidget->SetRightValue(DBManager::GetOutFolder(testItem->ownerTestCase->FullFileName,
                                                            testItem->RelativeFileName,
                                                            outMark));

        if(testItem->status->benchmarks.contains(rightRunNum))
        {
            if(testItem->status->benchmarks.value(rightRunNum).status.compare("perfect") == 0)
            {
                ui->rightPerfectBox->setChecked(true);
            }
            else
            {
                ui->rightTheBestBox->setChecked(true);
            }
        }
    }
    else
    {
        compareWidget->SetRightValue(QString());
    }
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

void MainWindow::saveExpandedState(QModelIndex &index)
{
    TestCaseFolder* folder = static_cast<TestCaseFolder*>(index.internalPointer());
    folder->Expanded = ui->testTreeView->isExpanded(index);

    for(int i = 0; i < folder->SubFolderList.length(); i++)
    {
        QModelIndex subIndex = testTreeAdapter.index(i, 0, index);
        saveExpandedState(subIndex);
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

void MainWindow::loadExpandedState(QModelIndex &index)
{
    TestCaseFolder* folder = static_cast<TestCaseFolder*>(index.internalPointer());
    ui->testTreeView->setExpanded(index, folder->Expanded);

    for(int i = 0; i < folder->SubFolderList.length(); i++)
    {
        QModelIndex subIndex = testTreeAdapter.index(i, 0, index);
        loadExpandedState(subIndex);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
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
            BeginMainWindowUpdate();
            testTreeAdapter.beginResetModel();

            MainWindowModel::OpenTestCase(tc);

            testTreeAdapter.endResetModel();
            EndMainWindowUpdate();
            loadTreeState();
        }
    }
}

void MainWindow::on_actionOpenTestCase_triggered()
{
    OpenTestCaseDialog openTestCaseDialog(this);
    openTestCaseDialog.exec();
    QList<TestCase *> *testCases = openTestCaseDialog.GetTestCaseList();

    saveTreeState();
    BeginMainWindowUpdate();
    testTreeAdapter.beginResetModel();

    for(int i = 0; i < testCases->count(); i++)
    {
        TestCase *tc = testCases->at(i);
        bool isLoadResult = tc->Checked;

        TestCaseFolder *tcf = MainWindowModel::OpenTestCase(tc);
        if((tcf != NULL) && isLoadResult)
        {
            //Load results
            MainWindowModel::LoadFolderResults(tcf);
        }
        else if(tcf == NULL)
        {
            //Test-case already open
        }
    }

    testTreeAdapter.endResetModel();
    EndMainWindowUpdate();
    loadTreeState();
}

void MainWindow::on_actionRun_dispatcher_triggered()
{
    PlanDispatcherDialog dispatcherTestPlanDialog(&MainWindowModel::tree,  this);
    dispatcherTestPlanDialog.exec();
}

void MainWindow::on_leftCompareComboBox_activated(int index)
{
    if(!isMainWindowUpdating)
    {
        TestCaseItem *testItem = GetCurrentTestItem();
        if(testItem == NULL) { return; }

        BeginMainWindowUpdate();

        FillLeftDetailPanel(testItem);

        EndMainWindowUpdate();
    }
}

void MainWindow::on_rightCompareComboBox_activated(int index)
{
    if(!isMainWindowUpdating)
    {
        TestCaseItem *testItem = GetCurrentTestItem();
        if(testItem == NULL) { return; }

        BeginMainWindowUpdate();

        FillRightDetailPanel(testItem);

        EndMainWindowUpdate();
    }
}

void MainWindow::UpdateTestStatus(int runNum, QString newStatus, bool checked)
{
    if(!isMainWindowUpdating)
    {
        TestCaseItem *testItem = GetCurrentTestItem();
        if(testItem == NULL) { return; }

        BeginMainWindowUpdate();
        testTableAdapter.beginResetModel();

        QMap<int, BenchmarkInfo> &currentTestBenchmarks = testItem->status->benchmarks;

        if(checked)
        {
            //Try to update benchmark status if benchmark is present in the list
            if(currentTestBenchmarks.contains(runNum))
            {
                if(currentTestBenchmarks.value(runNum).status.compare(newStatus) == 0) { return; }

                currentTestBenchmarks[runNum].status = newStatus;
            }
            else
            {
                //Benchmark not present, need to insert new benchmark
                BenchmarkInfo benchmarkInfo;

                benchmarkInfo.outMark = testItem->results.value(runNum)->outMark;
                benchmarkInfo.status = newStatus;

                currentTestBenchmarks.insert(runNum, benchmarkInfo);
            }
        }
        else
        {
            if(!currentTestBenchmarks.contains(runNum)) { return; }

            //Need delete benchmark
            currentTestBenchmarks.remove(runNum);
        }

        DBManager::SaveTestStatus(testItem->ownerTestCase->FullFileName, testItem->RelativeFileName, testItem->status);

        testTableAdapter.beginResetModel();
        EndMainWindowUpdate();
    }
}

void MainWindow::on_leftTheBestBox_stateChanged(int arg1)
{
    TestCaseItem *testItem = GetCurrentTestItem();
    if(testItem == NULL) { return; }

    int runNum = testItem->ownerTestCase->RunDescriptions.keys().at(ui->leftCompareComboBox->currentIndex());

    if(arg1 == Qt::Checked)
    {
        ui->leftPerfectBox->setChecked(false);
    }

    UpdateTestStatus(runNum, "thebest", arg1 == Qt::Checked);
}

void MainWindow::on_rightTheBestBox_stateChanged(int arg1)
{
    TestCaseItem *testItem = GetCurrentTestItem();
    if(testItem == NULL) { return; }

    int runNum = testItem->ownerTestCase->RunDescriptions.keys().at(ui->rightCompareComboBox->currentIndex());

    if(arg1 == Qt::Checked)
    {
        ui->rightPerfectBox->setChecked(false);
    }

    UpdateTestStatus(runNum, "thebest", arg1 == Qt::Checked);
}

void MainWindow::on_leftPerfectBox_stateChanged(int arg1)
{
    TestCaseItem *testItem = GetCurrentTestItem();
    if(testItem == NULL) { return; }

    int runNum = testItem->ownerTestCase->RunDescriptions.keys().at(ui->leftCompareComboBox->currentIndex());

    if(arg1 == Qt::Checked)
    {
        ui->leftTheBestBox->setChecked(false);
    }

    UpdateTestStatus(runNum, "perfect", arg1 == Qt::Checked);
}

void MainWindow::on_rightPerfectBox_stateChanged(int arg1)
{
    TestCaseItem *testItem = GetCurrentTestItem();
    if(testItem == NULL) { return; }

    int runNum = testItem->ownerTestCase->RunDescriptions.keys().at(ui->rightCompareComboBox->currentIndex());

    if(arg1 == Qt::Checked)
    {
        ui->rightTheBestBox->setChecked(false);
    }

    UpdateTestStatus(runNum, "perfect", arg1 == Qt::Checked);
}

void MainWindow::on_actionLoadAllResults_triggered()
{
    QModelIndex indexTable = ui->testTableView->currentIndex();

    BeginMainWindowUpdate();

    testTableAdapter.beginResetModel();

    for(int i = 0; i < MainWindowModel::tree.length(); i++)
    {
        //Load results
        MainWindowModel::LoadFolderResults(MainWindowModel::tree.at(i));
    }

    testTableAdapter.endResetModel();

    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadTestCaseResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    if(tcf != NULL)
    {
        while(tcf->ParentFolder != NULL) { tcf = tcf->ParentFolder; }
        //Load results
        MainWindowModel::LoadFolderResults(tcf);
    }

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadFolderResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    if(tcf != NULL)
    {
        //Load results
        MainWindowModel::LoadFolderResults(tcf);
    }

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadTestResult_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();
    TestCaseItem *test = tcf->TestList.at(indexTable.row());

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    if((tcf != NULL) && (test != NULL))
    {
        //Load results
        MainWindowModel::LoadTestResults(tcf, test);
    }

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadRunResults_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();
    int colRun = indexTable.column() - 1;
    if(colRun < 0) { return; }
    int keyRun = tcf->ownerTestCase->RunDescriptions.keys().at(colRun);
    RunDescription *run = tcf->ownerTestCase->RunDescriptions.value(keyRun);

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    if((tcf != NULL) && (run != NULL))
    {
        //Load results
        MainWindowModel::LoadRunResults(tcf, run);
    }

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionLoadOneResult_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();
    TestCaseItem *test = tcf->TestList.at(indexTable.row());

    int colRun = indexTable.column() - 1;
    if(colRun < 0) { return; }
    int keyRun = tcf->ownerTestCase->RunDescriptions.keys().at(colRun);
    RunDescription *run = tcf->ownerTestCase->RunDescriptions.value(keyRun);

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    if((tcf != NULL) && (test != NULL))
    {
        //Load results
        MainWindowModel::LoadOneResult(tcf, test, run);
    }

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_actionShowRun_triggered()
{
    QModelIndex indexTree = ui->testTreeView->currentIndex();
    TestCaseFolder *tcf = static_cast<TestCaseFolder*>(indexTree.internalPointer());

    QModelIndex indexTable = ui->testTableView->currentIndex();

    MainWindowModel::UpdateRunDescriptions(tcf->ownerTestCase);

    BeginMainWindowUpdate();
    testTableAdapter.beginResetModel();

    RunDescriptionShowDialog runDescriptionShowDialog(&(tcf->ownerTestCase->RunDescriptions), this);
    runDescriptionShowDialog.exec();

    testTableAdapter.endResetModel();
    EndMainWindowUpdate();

    ui->testTableView->setCurrentIndex(indexTable);
}

void MainWindow::on_testTableView_customContextMenuRequested(const QPoint &pos)
{
    tableContextMenu.popup(ui->testTableView->viewport()->mapToGlobal(pos));
}

void MainWindow::on_testTableHeaderView_customContextMenuRequested(const QPoint &pos)
{
    tableHeadContextMenu.popup(ui->testTableView->horizontalHeader()->viewport()->mapToGlobal(pos));
}

void MainWindow::on_testTreeView_customContextMenuRequested(const QPoint &pos)
{
    treeContextMenu.popup(ui->testTreeView->viewport()->mapToGlobal(pos));
}

// Tag operations
void MainWindow::on_menuTagCollections_triggered()
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

void MainWindow::on_menyAttachTag_triggered()
{
    AttachTag();
}

void MainWindow::on_attachTagBtn_clicked()
{
    AttachTag();
}

void MainWindow::AttachTag()
{
    TagDispatcherDialog dlg(TagDispatcherDialog::ACTION_SELECT, this);
    dlg.exec();

    TagItem *tagItem = dlg.getSelectResult();
    if((tagItem != NULL) && (tagItem->tag != NULL))
    {
        TestCaseItem *testItem = GetCurrentTestItem();
        if(testItem == NULL) { return; }

        testItem->status->tags.append(tagItem->path);
        UpdateTagListForCurrentTest();

        DBManager::SaveTestStatus(testItem->ownerTestCase->FullFileName, testItem->RelativeFileName, testItem->status);
    }
}

void MainWindow::on_tagBox_anchorClicked(const QUrl &arg1)
{
    QString s = arg1.toString();
    int p = s.indexOf("#remove");

    if(p > 0)
    {
        QMessageBox mb("Confirmation",
                       "Are you shure to remove tag link? Tag will not be deleted from collection.",
                       QMessageBox::Question,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::Cancel,
                       QMessageBox::NoButton);

        if(mb.exec() == QMessageBox::Ok)
        {
            TestCaseItem *testItem = GetCurrentTestItem();
            if(testItem == NULL) { return; }

            testItem->status->tags.removeAll(s.left(p));
            UpdateTagListForCurrentTest();

            DBManager::SaveTestStatus(testItem->ownerTestCase->FullFileName, testItem->RelativeFileName, testItem->status);
        }

        return;
    }

    p = s.indexOf("#view");

    if(p > 0)
    {
        TagViewDialog dlg;
        dlg.setTag(s.left(p));
        dlg.exec();
        return;
    }
}

void MainWindow::on_menuContentsHelp_triggered()
{
    HelpManager::showContents();
}

void MainWindow::on_menuIndexHelp_triggered()
{
    HelpManager::showIndex();
}
