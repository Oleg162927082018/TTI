#include "plan-add-dialog.h"
#include "ui_plan-add-dialog.h"

#include "Common/RunManager/runmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QString>

PlanAddDialog::PlanAddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanAddDialog)
{
    ui->setupUi(this);
}

PlanAddDialog::~PlanAddDialog()
{
    delete ui;
}

void PlanAddDialog::on_planBtn_clicked()
{
    QDir appFolder(qApp->applicationDirPath());
    appFolder.mkdir("plans");

    //Get file name of testing plan
    QString testingPlanFileName = QFileDialog::getOpenFileName(this, "Run Testing plan",
                                    appFolder.filePath("plans"), "Testing plan (*.pln)");

    if(testingPlanFileName.count() > 0)
    {
        ui->planBox->setText(testingPlanFileName);
    }
}

void PlanAddDialog::on_buttonBox_accepted()
{
    QDir appFolder(qApp->applicationDirPath());
    appFolder.mkdir("plans");

    //Get file name for testing plan
    QString testingPlanFileName = ui->planBox->text();
    RunManager::AddPlan(testingPlanFileName, ui->descriptionBox->toPlainText());

    close();
}
