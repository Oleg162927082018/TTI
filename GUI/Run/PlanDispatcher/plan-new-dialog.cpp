#include "plan-new-dialog.h"
#include "ui_plan-new-dialog.h"

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>



PlanNewDialog::PlanNewDialog(QList<TestCaseFolder *> *testCases, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanNewDialog)
{
    testCaseList = testCases;

    ui->setupUi(this);
}

PlanNewDialog::~PlanNewDialog()
{
    delete ui;
}

void PlanNewDialog::closeEvent(QCloseEvent *e)
{
    if(QFileInfo(ui->planBox->text()).exists())
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void PlanNewDialog::on_planBtn_clicked()
{
    QDir appFolder(qApp->applicationDirPath());
    appFolder.mkdir("plans");

    ui->planBox->setText(QFileDialog::getSaveFileName(this, "Run Testing plan",
                            appFolder.filePath("plans"), "Testing plan (*.pln)"));

}

void PlanNewDialog::on_PlanNewDialog_accepted()
{
    DBManager::SaveTestingPlan(ui->planBox->text(), testCaseList);
    RunManager::AddPlan(ui->planBox->text(), ui->descBox->text());
}
