#include "plan-dispatcher-dialog.h"
#include "ui_plan-dispatcher-dialog.h"

#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

#include "GUI/Run/PlanDispatcher/plan-add-dialog.h"
#include "GUI/Run/PlanDispatcher/plan-edit-dialog.h"
#include "GUI/Run/PlanDispatcher/plan-new-dialog.h"

#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>

PlanDispatcherDialog::PlanDispatcherDialog(QList<TestCaseFolder *> *testCasesCache, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDispatcherDialog)
{
    testCaseList = testCasesCache;
    logAdapter.Init(NULL);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    ui->setupUi(this);

    ui->queueTable->setModel(&taskTableAdapter);
    ui->queueTable->setStyleSheet("QTableView::item:selected{background-color: " +
                                  palette().highlight().color().name() +
                                  "; color: " + palette().highlightedText().color().name() + ";};");

    ui->logBox->setModel(&logAdapter);

    connect(RunManager::Handle, &RunManager::logChanged, this, &PlanDispatcherDialog::onLogChanged);
    connect(RunManager::Handle, &RunManager::testStarted, this, &PlanDispatcherDialog::onTestStarted);
    connect(RunManager::Handle, &RunManager::testFinished, this, &PlanDispatcherDialog::onTestFinished);
    connect(RunManager::Handle, &RunManager::testCaseStarted, this, &PlanDispatcherDialog::onTestCaseStarted);
    connect(RunManager::Handle, &RunManager::testCaseFinished, this, &PlanDispatcherDialog::onTestCaseFinished);
    connect(RunManager::Handle, &RunManager::planStarted, this, &PlanDispatcherDialog::onPlanStarted);
    connect(RunManager::Handle, &RunManager::planFinished, this, &PlanDispatcherDialog::onPlanFinished);
    connect(RunManager::Handle, &RunManager::allFinished, this, &PlanDispatcherDialog::onAllFinished);



    connect(ui->queueTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PlanDispatcherDialog::onQueueTableSelectionChanged);
}

PlanDispatcherDialog::~PlanDispatcherDialog()
{
    delete ui;
}

void PlanDispatcherDialog::on_newBtn_clicked()
{
    PlanNewDialog newPlanDialog(testCaseList, this);

    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();
    newPlanDialog.exec();
    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_addBtn_clicked()
{
    PlanAddDialog addPlanDialog(this);

    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();
    addPlanDialog.exec();
    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_editBtn_clicked()
{
    PlanEditDialog editPlanDialog(this);

    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();
    editPlanDialog.exec();
    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_removeBtn_clicked()
{
    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();

    int rowNum = ui->queueTable->currentIndex().row();
    int status = RunManager::GetPlan(rowNum)->status;
    if(status != PlanStatus::Prosessing)
    {
        RunManager::RemovePlan(rowNum);
        if(ind.row() > rowNum)
        {
            ind = taskTableAdapter.index(ind.row() - 1, ind.column(), ind);
        }
    }
    else
    {
        QMessageBox mb("Error",
                       "You need stop plan execution before remove it!",
                       QMessageBox::Warning,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();
    }

    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_clearBtn_clicked()
{
    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();
    logAdapter.beginResetModel();

    int i = 0; int indRow = ind.row();
    while(i < RunManager::GetQueueLength())
    {
        if(RunManager::GetPlan(i)->status == PlanStatus::Finished)
        {
            RunManager::RemovePlan(i);
            if(indRow > i) { indRow--; }
        }
        else
        {
            i++;
        }
    }

    logAdapter.endResetModel();
    taskTableAdapter.endResetModel();

    ind = taskTableAdapter.index(indRow - 1, ind.column(), ind);
    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_startBtn_clicked()
{
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);
    ui->pauseBtn->setEnabled(true);
    RunManager::Start(0); //ui->queueTable->currentIndex().row()
}

void PlanDispatcherDialog::on_stopBtn_clicked()
{
    ui->startBtn->setEnabled(true);
    ui->startBtn->setText("Start");
    ui->stopBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
    RunManager::Stop();
}

void PlanDispatcherDialog::on_pauseBtn_clicked()
{
    ui->startBtn->setEnabled(true);
    ui->startBtn->setText("Resume");
    ui->stopBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
    RunManager::Pause();
}


void PlanDispatcherDialog::on_moveUpBtn_clicked()
{
    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();

    int rowNum = ui->queueTable->currentIndex().row();
    if(rowNum > 0)
    {
        RunManager::MoveUp(rowNum);
        ind = taskTableAdapter.index(ind.row() - 1, ind.column(), ind);
    }

    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::on_moveDownBtn_clicked()
{
    QModelIndex ind = ui->queueTable->currentIndex();

    taskTableAdapter.beginResetModel();

    int rowNum = ind.row();
    if((rowNum >= 0) && (rowNum < (taskTableAdapter.rowCount() - 1)))
    {
        RunManager::MoveDown(rowNum);
        ind = taskTableAdapter.index(ind.row() + 1, ind.column(), ind);
    }

    taskTableAdapter.endResetModel();

    ui->queueTable->setCurrentIndex(ind);
}

void PlanDispatcherDialog::onQueueTableSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    int row = newSelection.at(0).indexes().at(0).row();

    logAdapter.beginResetModel();

    logAdapter.Init(&(RunManager::GetPlan(row)->log));

    int testCount = RunManager::GetPlan(row)->testCount;
    ui->progressBar->setMaximum(testCount>0?testCount:1);
    ui->progressBar->setValue(RunManager::GetPlan(row)->testFinished);

    logAdapter.endResetModel();
}

void PlanDispatcherDialog::onLogChanged(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {
        logAdapter.beginResetModel();
        logAdapter.endResetModel();
    }
}

void PlanDispatcherDialog::onTestStarted(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {

    }
}

void PlanDispatcherDialog::onTestFinished(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {
        ui->progressBar->setMaximum(RunManager::GetPlan(ui->queueTable->currentIndex().row())->testCount);
        ui->progressBar->setValue(RunManager::GetPlan(ui->queueTable->currentIndex().row())->testFinished);
    }
}

void PlanDispatcherDialog::onTestCaseStarted(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {

    }
}

void PlanDispatcherDialog::onTestCaseFinished(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {

    }
}

void PlanDispatcherDialog::onPlanStarted(int planInd)
{
    taskTableAdapter.beginResetModel();
    taskTableAdapter.endResetModel();

    QModelIndex idx = taskTableAdapter.index(planInd, 0);
    ui->queueTable->setCurrentIndex(idx);
}

void PlanDispatcherDialog::onPlanFinished(int planInd)
{
    if(ui->queueTable->currentIndex().row() == planInd)
    {
        taskTableAdapter.beginResetModel();
        taskTableAdapter.endResetModel();

        QModelIndex idx = taskTableAdapter.index(planInd, 0);
        ui->queueTable->setCurrentIndex(idx);

        logAdapter.beginResetModel();
        logAdapter.endResetModel();
    }
}

void PlanDispatcherDialog::onAllFinished()
{
    ui->startBtn->setEnabled(true);
    ui->startBtn->setText("Start");
    ui->stopBtn->setEnabled(false);
    ui->pauseBtn->setEnabled(false);
}

void PlanDispatcherDialog::on_hideBtn_clicked()
{
    ui->logWidget->setMaximumHeight(ui->logHeaderWidget->height());

    ui->logBox->hide();
    ui->queueWidget->show();
}

void PlanDispatcherDialog::on_showLog44Btn_clicked()
{
    ui->logWidget->setMaximumHeight(16777215);

    ui->queueWidget->hide();
    ui->logBox->show();
}

void PlanDispatcherDialog::on_showLog34Btn_clicked()
{
    ui->logWidget->setMaximumHeight(16777215);

    int H = ui->splitter->height() - ui->splitter->handleWidth();
    ui->splitter->setSizes(QList<int>{(int)(H*0.25),int(H*0.75)});

    ui->queueWidget->show();
    ui->logBox->show();
}

void PlanDispatcherDialog::on_showLog24Btn_clicked()
{
    ui->logWidget->setMaximumHeight(16777215);

    int H = ui->splitter->height() - ui->splitter->handleWidth();
    ui->splitter->setSizes(QList<int>{(int)(H*0.5),int(H*0.5)});

    ui->queueWidget->show();
    ui->logBox->show();
}

void PlanDispatcherDialog::on_showLog14Btn_clicked()
{
    ui->logWidget->setMaximumHeight(16777215);

    int H = ui->splitter->height() - ui->splitter->handleWidth();
    ui->splitter->setSizes(QList<int>{(int)(H*0.75),int(H*0.25)});

    ui->queueWidget->show();
    ui->logBox->show();
}

void PlanDispatcherDialog::on_closeBtn_clicked()
{
    close();
}
