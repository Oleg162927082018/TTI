#include "create-testcase-dialog.h"
#include "ui_create-testcase-dialog.h"

#include "create-testcase-configuration-one-widget.h"
#include "create-testcase-configuration-list-widget.h"

#include "Common/DllManager/dllmanager.h"

#include <QFileInfo>

CreateTestCaseDialog::CreateTestCaseDialog(QString fileName, QStringList IDs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTestCaseDialog)
{
    ui->setupUi(this);

    testCaseFullFileName = fileName;
    testCaseIDs = IDs;

    templateListAdapter.Init(&testCaseIDs);
    ui->templateList->setModel(&templateListAdapter);

    foreach(QString id, testCaseIDs)
    {
        CreateTestCaseConfigurationListWidget *templateDetailWidget = new CreateTestCaseConfigurationListWidget(id, this);
        ui->templateConfigurationsStack->addWidget(templateDetailWidget);
    }

    connect(ui->templateList->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          this, SLOT(on_templateSelectionChanged(QItemSelection, QItemSelection)));
}

CreateTestCaseDialog::~CreateTestCaseDialog()
{
    delete ui;
}

void CreateTestCaseDialog::on_CancelButton_clicked()
{
    reject();
}

void CreateTestCaseDialog::on_OkButton_clicked()
{
    QWidget *widget = ui->templateConfigurationsStack->currentWidget();
    CreateTestCaseConfigurationListWidget *subWidget = static_cast<CreateTestCaseConfigurationListWidget *>(widget);

    if(subWidget->SaveTestCase(testCaseFullFileName)) { accept(); }
}

void CreateTestCaseDialog::on_templateSelectionChanged(const QItemSelection &newSelection, const QItemSelection &oldSelection)
{
    QModelIndex firstIndex = newSelection.at(0).indexes().at(0);
    ui->templateConfigurationsStack->setCurrentIndex(firstIndex.row());

    ITestCaseAdapter *tc = DLLManager::GetTestCaseAdapter(testCaseIDs.at(firstIndex.row()));
    ui->templateDescriptionBox->setText(tc->Description());
}

TestCase *CreateTestCaseDialog::GetTestCase()
{
    QFileInfo tcInf(testCaseFullFileName);
    if(tcInf.exists())
    {
        //Load Test-Case
        TestCase *tc = DBManager::GetTestCase(tcInf.filePath());
        tc->Checked = false; //is load results immediately
        tc->Name = tcInf.fileName();
        tc->FullFileName = tcInf.filePath();

        return tc;
    }
    else
    {
        return NULL;
    }
}
