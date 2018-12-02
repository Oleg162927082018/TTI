#include "plan-new-dialog.h"
#include "ui_plan-new-dialog.h"

#include "plan-new-testcase-override-widget.h"

#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>



PlanNewDialog::PlanNewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanNewDialog)
{
    ui->setupUi(this);

    testCaseListAdapter.Init(&testCaseNames);
    ui->testCaseListView->setModel(&testCaseListAdapter);
}

PlanNewDialog::~PlanNewDialog()
{
    delete ui;

    for(int i = 0; i < overrideTestCases.count(); i++) { delete overrideTestCases.at(i); }
}

void PlanNewDialog::addTest(TestCase *testCase, TestStatus * testStatus)
{
    if(!originalTestCases.contains(testCase)) {
        testCaseListAdapter.beginResetModel();

        testCaseNames.append(testCase->fullFileName.mid(testCase->fullFileName.lastIndexOf("/") + 1));
        originalTestCases.append(testCase);
        overrideTestCases.append(new TestCase(*testCase));

        ui->testCaseStackedWidget->addWidget(new PlanNewTestCaseOverrideWidget(this));

        testCaseListAdapter.endResetModel();
    }

    TestCase *overrideTestCase = overrideTestCases.at(originalTestCases.indexOf(testCase));
    tests.append(QPair<TestCase *, TestStatus *>(overrideTestCase, testStatus));
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

    DBManager::SaveTestingPlan(ui->fileNameBox->text(), &overrideTestCases, &tests,
                               ui->isDeleteAfterRunBox->isChecked(),
                               ui->isUpdateLastBox->isChecked());

    if(ui->isAddImediatelyBox->isChecked())
    {
        RunManager::AddPlan(ui->fileNameBox->text(), ui->descBox->toPlainText());
    }
}
