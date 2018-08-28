#include "open-testcase-dialog.h"
#include "ui_open-testcase-dialog.h"

#include "GUI/MainWindow/Model/mainwindow-model.h"

#include <QFileDialog>

OpenTestCaseDialog::OpenTestCaseDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::OpenTestCaseDialog)
{
    ui->setupUi(this);

    runDescriptionTableWidget = new RunDescriptionTableWidget(this);
    ui->resultsLayout->addWidget(runDescriptionTableWidget);

    testCaseListController.setDataSource(&dataModel);
    ui->testCaseList->setModel(&testCaseListController);

    connect(ui->testCaseList->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_testCaseSelectionChanged(QItemSelection,QItemSelection)));
}

OpenTestCaseDialog::~OpenTestCaseDialog()
{
}

QList<TestCase *> *OpenTestCaseDialog::GetTestCaseList()
{
    return &(dataModel.testCaseList);
}

void OpenTestCaseDialog::on_addBtn_clicked()
{
    //Create default folder for save test cases
    QDir appFolder(qApp->applicationDirPath() + "/tests");
    if(!appFolder.exists()) { appFolder.mkdir("."); }

    //Get files for open
    QStringList testCaseFileNames = QFileDialog::getOpenFileNames(this, "Open Test cases",
                                    appFolder.filePath(""), "Test case (*.tcs)");

    if(testCaseFileNames.count() > 0)
    {
        BeginDialogUpdate();
        testCaseListController.beginResetModel();

        for(int i = 0; i < testCaseFileNames.count(); i++)
        {
            //Check if test-case already appended
            if(!dataModel.isTestCaseContained(testCaseFileNames.at(i)))
            {
                dataModel.addTestCase(testCaseFileNames.at(i));
            }
        }

        testCaseListController.endResetModel();
        EndDialogUpdate();

        QModelIndex index;
        if(dataModel.currentTestCase == NULL)
        {
            index = testCaseListController.index(0);
        }
        else
        {
            int n = dataModel.testCaseList.indexOf(dataModel.currentTestCase);
            index = testCaseListController.index(n);
        }
        ui->testCaseList->setCurrentIndex(index);
    }
}

void OpenTestCaseDialog::on_delBtn_clicked()
{
    int ind = ui->testCaseList->currentIndex().row();
    if(ind >= 0)
    {
        BeginDialogUpdate();
        testCaseListController.beginResetModel();

        dataModel.removeTestCase(ind);

        testCaseListController.endResetModel();
        EndDialogUpdate();

        if(dataModel.testCaseList.length() > 0)
        {
            QModelIndex index;
            if(dataModel.currentTestCase == NULL)
            {
                index = testCaseListController.index(0);
            }
            else
            {
                int n = dataModel.testCaseList.indexOf(dataModel.currentTestCase);
                index = testCaseListController.index(n);
            }
            ui->testCaseList->setCurrentIndex(index);
        }
        else
        {
            ui->fullFileNameBox->setText("");
            ui->currentConfigBox->clear();
            ui->currentConfigBox->clearEditText();
            runDescriptionTableWidget->SetDataSource(NULL);
        }
    }
}

void OpenTestCaseDialog::BeginDialogUpdate()
{
    isDialogUpdating = true;
}

void OpenTestCaseDialog::EndDialogUpdate()
{
    isDialogUpdating = false;
}

void OpenTestCaseDialog::on_currentConfigBox_currentIndexChanged(const QString &arg1)
{
    if(!isDialogUpdating)
    {
        if(dataModel.currentTestCase != NULL )
        {
            dataModel.currentTestCase->CurrentConfigName = arg1;
        }
    }
}

void OpenTestCaseDialog::on_isLoadResultBox_stateChanged(int arg1)
{
    if(!isDialogUpdating)
    {
        if(dataModel.currentTestCase != NULL )
        {
            dataModel.currentTestCase->Checked = (ui->isLoadResultBox->checkState() == Qt::Checked);
        }
    }
}

void OpenTestCaseDialog::on_testCaseSelectionChanged(const QItemSelection& newSelection, const QItemSelection &oldSelection)
{
    BeginDialogUpdate();

    if(!newSelection.indexes().isEmpty())
    {
        dataModel.currentTestCase = dataModel.testCaseList.at(newSelection.indexes().first().row());

        ui->fullFileNameBox->setText(dataModel.currentTestCase->FullFileName);
        ui->currentConfigBox->clear();
        for(int i = 0; i < dataModel.currentTestCase->ConfigList.keys().count(); i++)
        {
            ui->currentConfigBox->addItem(dataModel.currentTestCase->ConfigList.keys().at(i));
        }
        ui->currentConfigBox->setCurrentText(dataModel.currentTestCase->CurrentConfigName);
        ui->isLoadResultBox->setChecked(dataModel.currentTestCase->Checked);

        runDescriptionTableWidget->SetDataSource(&(dataModel.currentTestCase->RunDescriptions));
    }

    EndDialogUpdate();
}
