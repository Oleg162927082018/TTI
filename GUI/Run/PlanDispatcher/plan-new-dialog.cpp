#include "plan-new-dialog.h"
#include "ui_plan-new-dialog.h"

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>



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

void PlanNewDialog::on_isAddImediatelyBox_stateChanged(int arg1)
{
    ui->descBox->setEnabled(arg1 > 0);
}

void PlanNewDialog::on_fileNameBtn_clicked()
{
    QDir appFolder(qApp->applicationDirPath());
    appFolder.mkdir("plans");

    ui->fileNameBox->setText(QFileDialog::getSaveFileName(this, "Run Testing plan",
                            appFolder.filePath("plans"), "Testing plan (*.pln)"));
}

void PlanNewDialog::on_okCancelBtns_accepted()
{
    if(ui->fileNameBox->text().isEmpty())
    {
        QMessageBox mb("Error",
                       "Field File name must be fill by a valid file name!",
                       QMessageBox::Warning,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();

        return;
    }

    DBManager::SaveTestingPlan(ui->fileNameBox->text(), testCaseList,
                               ui->isDeleteAfterRunBox->isChecked(),
                               ui->isUpdateLastBox->isChecked());

    if(ui->isAddImediatelyBox->isChecked())
    {
        RunManager::AddPlan(ui->fileNameBox->text(), ui->descBox->toPlainText());
    }
}
