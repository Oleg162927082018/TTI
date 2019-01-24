#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Common/DllManager/dllmanager.h"
#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

#include "GUI/MainWindow/Model/mainwindow-model.h"
#include "GUI/MainWindow/Model/mainwindow-treefolder.h"
#include "GUI/MainWindow/Model/mainwindow-tableitem.h"
#include "GUI/Run/PlanDispatcher/View/plan-dispatcher-dialog.h"
#include "GUI/TestCase/Create/View/create-testcase-dialog.h"
#include "GUI/TestCase/Open/View/open-testcase-dialog.h"
#include "GUI/RunHeader/RunHeadersDialog/View/run-headers-dialog.h"
#include "GUI/Tag/Dispatcher/View/tag-dispatcher-dialog.h"
#include "GUI/Tag/View/View/tag-view-dialog.h"
#include "GUI/Search/View/set-filter-dialog.h"

#include "TagManager/tagmanager.h"
#include "HelpManager/helpmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QtHelp/QHelpEngineCore>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindowModel::testTableAdapter.Init(nullptr, nullptr);

    ui->testTreeView->setModel(&MainWindowModel::testTreeAdapter);
    ui->testTableView->setModel(&MainWindowModel::testTableAdapter);

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
        if(compareWidget != nullptr)
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


    // Tree context menu
    QAction *folderLoadAction = new QAction("Load", this);
    treeContextMenu.addAction(folderLoadAction);
    connect(folderLoadAction, SIGNAL(triggered()),
          this, SLOT(on_actionLoadFolderResults_triggered()));

    //Table head context menu
    ui->testTableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->testTableView->horizontalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(on_testTableHeaderView_customContextMenuRequested(QPoint)));

    QAction *hideRunAction = new QAction("Hide Run", this);
    tableHeadContextMenu.addAction(hideRunAction);

    QAction *showRunAction = new QAction("Show Runs", this);
    tableHeadContextMenu.addAction(showRunAction);
    connect(showRunAction, SIGNAL(triggered()),
          this, SLOT(on_actionShow_RunHeader_triggered()));

    QAction *descRunAction = new QAction("edit Run Description", this);
    tableHeadContextMenu.addAction(descRunAction);

    // Table head click
    connect(ui->testTableView->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(on_testTableHeaderClicked(int)));
}

MainWindow::~MainWindow()
{
    if(filterDlg != nullptr) { delete filterDlg; }

    DLLManager::FreeResources();
    RunManager::FreeResources();
    TagManager::FreeResources();
    HelpManager::FreeResources();

    delete ui;

    // Delete data model
    MainWindowModel::DeleteTree();
}

PlanNewDialog *MainWindow::createPlanNewDialog(QWidget *parent)
{
    PlanNewDialog *dlg = new PlanNewDialog(parent);
    for(int i = 0; i < MainWindowModel::tree.count(); i++) {
        MainWindowTreeFolder *mwRootTreeFolder = MainWindowModel::tree.at(i);
        if(mwRootTreeFolder->checkState != Qt::Unchecked) {
            for(int j = 0; j < mwRootTreeFolder->fullTableItems.count(); j++) {
                MainWindowTableItem *mwItem = mwRootTreeFolder->fullTableItems.at(j);
                if(mwItem->checked) {
                    dlg->addTest(mwItem->ownerTestCase, mwItem->status);
                }
            }
        }
    }
    return dlg;
}

//Begin section Tree-Table-ComboBoxes-Compare Panels chain
void MainWindow::on_testTreeViewSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection)
{
    //Try to get new data sources
    MainWindowTreeFolder *newTestCaseFolder = nullptr;
    if(newSelection.length() > 0) {
        QModelIndex newTreeIndex = newSelection.at(0).indexes().at(0);
        newTestCaseFolder = static_cast<MainWindowTreeFolder*>(newTreeIndex.internalPointer());
    }

    if(newTestCaseFolder != nullptr) {
        if(MainWindowModel::testTableAdapter.isInitNeeded(&(newTestCaseFolder->visibleTableItems),
                                         newTestCaseFolder->visibleTableHeaders)) {
            //Save old position
            MainWindowTreeFolder *oldTestCaseFolder = nullptr;
            MainWindowTableItem *oldTableItem =nullptr;
            MainWindowTableHeader *oldTableHeader = nullptr;

            if(oldSelection.length() > 0) {
                QModelIndex oldTreeIndex = oldSelection.at(0).indexes().at(0);
                oldTestCaseFolder = static_cast<MainWindowTreeFolder*>(oldTreeIndex.internalPointer());
            }
            if(oldTestCaseFolder != nullptr) {
                QModelIndex oldTableInd = ui->testTableView->currentIndex();
                int oldRow = oldTableInd.row();
                if((0 <= oldRow) && (oldRow < oldTestCaseFolder->visibleTableItems.count())) {
                    int key = oldTestCaseFolder->visibleTableItems.keys().at(oldRow);
                    oldTableItem = oldTestCaseFolder->visibleTableItems.value(key);
                }
                int oldCol = oldTableInd.column() - 1; // -1 because first column is name of test
                if((0 <= oldCol) && (oldCol < oldTestCaseFolder->visibleTableHeaders->count())) {
                    int key = oldTestCaseFolder->visibleTableHeaders->keys().at(oldCol);
                    oldTableHeader = oldTestCaseFolder->visibleTableHeaders->value(key);
                }
            }

            //Set new data sources
            MainWindowModel::testTableAdapter.beginResetModel();
            MainWindowModel::testTableAdapter.Init(&(newTestCaseFolder->visibleTableItems),
                                  newTestCaseFolder->visibleTableHeaders);
            MainWindowModel::testTableAdapter.endResetModel();

            //Restore old position
            int newTableRow = -1;
            if(oldTableItem != nullptr) {
                int key = newTestCaseFolder->visibleTableItems.key(oldTableItem);
                newTableRow = newTestCaseFolder->visibleTableItems.keys().indexOf(key);
            }
            if((newTableRow < 0) && (newTestCaseFolder->visibleTableItems.count() > 0)) { newTableRow = 0; }

            int newTableCol = 0;
            int newTableColKey = 0;
            if(oldTableHeader != nullptr) { newTableColKey = newTestCaseFolder->visibleTableHeaders->key(oldTableHeader); }
            if(newTableColKey > 0) { newTableCol = newTestCaseFolder->visibleTableHeaders->keys().indexOf(newTableColKey) + 1; }

            //Emit on_testTableViewSelectionChanged
            if(newTableRow >= 0) {
                QModelIndex newTableInd = MainWindowModel::testTableAdapter.index(newTableRow, newTableCol);
                ui->testTableView->setCurrentIndex(newTableInd);
            } else {
                emit this->on_testTableViewSelectionChanged(QItemSelection(), QItemSelection());
            }
        } //else { visibleTableItems and visibleTableHeaders not changed => nothing to do }
    } else {
        MainWindowModel::testTableAdapter.beginResetModel();
        MainWindowModel::testTableAdapter.Init(nullptr, nullptr);
        MainWindowModel::testTableAdapter.endResetModel();

        //Emit on_testTableViewSelectionChanged
        emit this->on_testTableViewSelectionChanged(QItemSelection(), QItemSelection());
    }
}

void MainWindow::on_testTableViewSelectionChanged(const QItemSelection &newSelection, const QItemSelection& oldSelection)
{
    //Get new selected test
    MainWindowTableItem *newTableItem = nullptr;
    if(newSelection.length() > 0)
    {
        //Get new selected test
        QModelIndex newTableIndex = newSelection.at(0).indexes().at(0);
        newTableItem = MainWindowModel::testTableAdapter.getRowData(newTableIndex.row());
    }

    if((newTableItem != nullptr) && (newTableItem->fullTableHeaders->count() > 0)) {

        int leftInd = ui->leftCompareComboBox->currentIndex();
        int rightInd = ui->rightCompareComboBox->currentIndex();
        QString benchmarkHeaders[] = { "current Benchmark", "compared Benchmark" };

        if((newTableItem->fullTableHeaders != compareComboBox_dataSource) ||
                (compareComboBox_dataSource->count() > (ui->leftCompareComboBox->count() - benchmarkHeaders->count()) )) {

            //Reload the left and right ComboBoxes
            compareComboBox_dataSource = newTableItem->fullTableHeaders;

            ui->leftCompareComboBox->clear();
            ui->rightCompareComboBox->clear();

            ui->leftCompareComboBox->addItem(benchmarkHeaders[0], QVariant(0));
            ui->leftCompareComboBox->addItem(benchmarkHeaders[1], QVariant(0));
            ui->rightCompareComboBox->addItem(benchmarkHeaders[0], QVariant(0));
            ui->rightCompareComboBox->addItem(benchmarkHeaders[1], QVariant(0));

            for(int i = newTableItem->fullTableHeaders->count() - 1; i >= 0; i--)
            {
                int runNum = newTableItem->fullTableHeaders->keys().at(i);
                ui->leftCompareComboBox->addItem(compareComboBox_dataSource->value(runNum)->name, QVariant(runNum));
                ui->rightCompareComboBox->addItem(compareComboBox_dataSource->value(runNum)->name, QVariant(runNum));
            }

            //Setup extra compare widget
            IResultCompareWidget *newCompareWidget = compareWidgets.value(newTableItem->ownerTestCase->ID);
            if(newCompareWidget != nullptr)
            {
                ui->extraCompareWidget->setCurrentWidget(newCompareWidget);
            }
            else
            {
                 ui->extraCompareWidget->setCurrentIndex(0);
            }
        }

        if((leftInd < 0) || (leftInd >=  ui->leftCompareComboBox->count())) { leftInd = 1; }
        if((rightInd < 0) || (rightInd >=  ui->rightCompareComboBox->count())) { rightInd = ui->rightCompareComboBox->count() - 1; }

        //Raise on_leftCompareComboBox_currentIndexChanged
        ui->leftCompareComboBox->setCurrentIndex(leftInd);
        emit this->on_leftCompareComboBox_activated(leftInd);
        //Raise on_rightCompareComboBox_currentIndexChanged
        ui->rightCompareComboBox->setCurrentIndex(rightInd);
        emit this->on_rightCompareComboBox_activated(rightInd);

    } else {
        compareComboBox_dataSource = nullptr;

        ui->leftCompareComboBox->clear();
        ui->rightCompareComboBox->clear();

        ui->extraCompareWidget->setCurrentIndex(0);

        //Raise on_leftCompareComboBox_currentIndexChanged
        emit this->on_leftCompareComboBox_activated(-1);
        //Raise on_rightCompareComboBox_currentIndexChanged
        emit this->on_rightCompareComboBox_activated(-1);
    }

    updateTagList(newTableItem);
}

void MainWindow::updateTagList(MainWindowTableItem *tableItem)
{
    if(tableItem != nullptr) {

        //Temporary code for displaying tags
        QString tagList = "<html><head></head><body>";
        QString concatenator = "";

        for(int i = 0; i < tableItem->status->tags.count(); i++)
        {
            QString tagLink = tableItem->status->tags.at(i);
            QString tagName = tagLink.mid(tagLink.lastIndexOf('/') + 1);
            tagList +=  concatenator + "<span style=\" font-size:8.25pt; text-decoration: underline; color:#0000ff;\"><a href=\"" +
                        tagLink + "#view\">" + tagName + "</a> [<a href=\"" + tagLink + "#remove\">Х</a>]</span>";
            concatenator = "<span style=\" font-size:8.25pt;\">; </span>";
        }
        tagList += "</body></html>";

        ui->tagBox->clear();
        ui->tagBox->setHtml(tagList);
    } else {

        ui->tagBox->clear();
    }
}



void MainWindow::on_testTableHeaderClicked(int arg1)
{
    if(arg1 == 0)
    {
        QModelIndex currTreeInd = ui->testTreeView->currentIndex();
        MainWindowTreeFolder *currTreeFolder = static_cast<MainWindowTreeFolder*>(currTreeInd.internalPointer());
        MainWindowModel::setCheckState(currTreeFolder);
    }
}

void MainWindow::updateComparePanel(int index, QComboBox *compareComboBox, QPlainTextEdit *logBox, QLabel *infoLabel,
    QCheckBox *perfectBox, QCheckBox *theBestBox, SideOfCompareWidget sideOfCompareWidget)
{
    //Get table item
    QModelIndex tableIndex = ui->testTableView->currentIndex();
    MainWindowTableItem *testItem = MainWindowModel::testTableAdapter.getRowData(tableIndex.row());

    // Get widget
    IResultCompareWidget *compareWidget = nullptr;
    if(testItem != nullptr) { compareWidget = compareWidgets.value(testItem->ownerTestCase->ID); }

    //Get run num
    int runNum = 0;
    if(index >= 0) { runNum = compareComboBox->itemData(index).toInt(); }

    //Set log data
    if((testItem != nullptr) && (runNum > 0)) {

        QByteArray logData = DBManager::GetConsoleLog(testItem->ownerTestCase->fullFileName,
                                                      testItem->status->relativeFileName,
                                                      DBManager::GetRunName(runNum));

        logBox->setPlainText(logData);
    } else {

        logBox->clear();
    }

    //Get test result
    TestResult *testResult = nullptr;
    if(runNum > 0) { testResult = testItem->results.value(runNum); }
    if((testResult == nullptr) && (testItem != nullptr) && (runNum > 0)) {
        testResult = DBManager::GetTestResult(testItem->ownerTestCase->fullFileName, testItem->status->relativeFileName, DBManager::GetRunName(runNum));
    }

    if((testResult != nullptr) && (testItem != nullptr) && (runNum > 0)){
        if(testItem->status->benchmarks.contains(runNum)) {
            infoLabel->setStyleSheet("background-color: #008080");
            infoLabel->setText("Benchmark updated");
        } else {
            infoLabel->setStyleSheet("background-color: " + testResult->color.name());
            infoLabel->setText(testResult->status);
        }
    } else {
        infoLabel->setStyleSheet("");
        infoLabel->clear();
    }

    //Set left value
    if((compareWidget != nullptr) && (testItem != nullptr) && (testResult != nullptr)) {

        //Set output data
        if(sideOfCompareWidget == LeftSideOfCompareWidget) {
            compareWidget->SetLeftValue(DBManager::GetOutFolder(testItem->ownerTestCase->fullFileName,
                                                                testItem->status->relativeFileName,
                                                                testResult->outMark));
        } else {
            compareWidget->SetRightValue(DBManager::GetOutFolder(testItem->ownerTestCase->fullFileName,
                                                                 testItem->status->relativeFileName,
                                                                 testResult->outMark));
        }
    } else if(compareWidget != nullptr) {
        if(sideOfCompareWidget == LeftSideOfCompareWidget) {
            compareWidget->ClearLeftValue();
        } else {
            compareWidget->ClearRightValue();
        }
    }

    //Set perfect and the best check boxes
    perfectBox->setChecked(false);
    theBestBox->setChecked(false);
    if((testItem != nullptr) && (runNum > 0)) {
        if(testItem->status->benchmarks.contains(runNum))
        {
            if(testItem->status->benchmarks.value(runNum).status.compare("perfect") == 0) {
                perfectBox->setChecked(true);
            } else { theBestBox->setChecked(true); }
        }
    }

    //Need to append fill if user select "Current Benchmark" and "Compared Benchmark"
}

void MainWindow::on_leftCompareComboBox_activated(int index)
{
    updateComparePanel(index, ui->leftCompareComboBox, ui->leftLogBox, ui->leftMainInfoLabel,
                       ui->leftPerfectBox, ui->leftTheBestBox, LeftSideOfCompareWidget);
}

void MainWindow::on_rightCompareComboBox_activated(int index)
{
    updateComparePanel(index, ui->rightCompareComboBox, ui->rightLogBox, ui->rightMainInfoLabel,
                       ui->rightPerfectBox, ui->rightTheBestBox, RightSideOfCompareWidget);
}

//Controls events

void MainWindow::on_leftPerfectBox_clicked(bool checked)
{
    //Get run num
    int runNum = 0;
    int index = ui->leftCompareComboBox->currentIndex();
    if(index >= 0) { runNum = ui->leftCompareComboBox->itemData(index).toInt(); }

    if(runNum > 0) {
        updateTestStatus(runNum, "perfect", checked);
    }
}

void MainWindow::on_leftTheBestBox_clicked(bool checked)
{
    //Get run num
    int runNum = 0;
    int index = ui->leftCompareComboBox->currentIndex();
    if(index >= 0) { runNum = ui->leftCompareComboBox->itemData(index).toInt(); }

    if(runNum > 0) {
        updateTestStatus(runNum, "thebest", checked);
    }
}

void MainWindow::on_rightPerfectBox_clicked(bool checked)
{
    //Get run num
    int runNum = 0;
    int index = ui->rightCompareComboBox->currentIndex();
    if(index >= 0) { runNum = ui->rightCompareComboBox->itemData(index).toInt(); }

    if(runNum > 0) {
        updateTestStatus(runNum, "perfect", checked);
    }
}

void MainWindow::on_rightTheBestBox_clicked(bool checked)
{
    //Get run num
    int runNum = 0;
    int index = ui->rightCompareComboBox->currentIndex();
    if(index >= 0) { runNum = ui->rightCompareComboBox->itemData(index).toInt(); }

    if(runNum > 0) {
        updateTestStatus(runNum, "thebest", checked);
    }
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
void MainWindow::on_attachTagBtn_clicked()
{
    attachTag();
}

void MainWindow::on_tagBox_anchorClicked(const QUrl &arg1)
{
    QString s = arg1.toString();
    if(s.endsWith("#remove"))
    {

        QMessageBox mb("Confirmation",
                       "Are you shure to remove tag link? Tag will not be deleted from collection.",
                       QMessageBox::Question,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::Cancel,
                       QMessageBox::NoButton);

        if(mb.exec() == QMessageBox::Ok)
        {
            //Get table item
            QModelIndex tableIndex = ui->testTableView->currentIndex();
            MainWindowTableItem *testItem = MainWindowModel::testTableAdapter.getRowData(tableIndex.row());
            if(testItem == nullptr) { return; }

            testItem->status->tags.removeAll(s.left(s.lastIndexOf('#')));
            DBManager::SaveTestStatus(testItem->ownerTestCase->fullFileName, testItem->status);
            updateTagList(testItem);
        }

    } else if(s.endsWith("#view")) {

        TagViewDialog dlg(this);
        dlg.displayTag(s.left(s.lastIndexOf('#')));
    }
}
