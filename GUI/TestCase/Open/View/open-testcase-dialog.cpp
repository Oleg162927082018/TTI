#include "open-testcase-dialog.h"
#include "ui_open-testcase-dialog.h"

#include "GUI/MainWindow/Model/mainwindow-model.h"

#include <QFileDialog>

OpenTestCaseDialog::OpenTestCaseDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::OpenTestCaseDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);

    openTestCaseListController.dataSource = &(dataModel.itemList);
    ui->testCaseList->setModel(&openTestCaseListController);


    connect(ui->testCaseList->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
          this, SLOT(on_testCaseSelectionChanged(QItemSelection,QItemSelection)));

    ui->runDescriptionTable->setModel(&openTestCaseRunDescriptionsController);
    ui->runDescriptionTable->setColumnWidth(0, 50);
    ui->runDescriptionTable->setColumnWidth(1, 140);
    ui->runDescriptionTable->setColumnWidth(2, 200);

    connect(ui->runDescriptionTable->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(on_runDescriptionTableHeaderClicked(int)));
}

OpenTestCaseDialog::~OpenTestCaseDialog()
{
}

QList<OpenTestCaseItem *> &OpenTestCaseDialog::GetOpenTestCaseItemList()
{
    return dataModel.itemList;
}

void OpenTestCaseDialog::on_testCaseSelectionChanged(const QItemSelection& newSelection, const QItemSelection &oldSelection)
{
    if(!newSelection.indexes().isEmpty())
    {
        OpenTestCaseItem *tcItem = dataModel.itemList.at(newSelection.indexes().first().row());
        if(dataModel.curItem != tcItem) {

            dataModel.curItem = tcItem;

            if(dataModel.curItem != nullptr) {

                openTestCaseRunDescriptionsController.beginResetModel();
                openTestCaseRunDescriptionsController.dataSource = &(dataModel.curItem->visibleRunDescriptions);
                openTestCaseRunDescriptionsController.endResetModel();
                QModelIndex firstRunDescriptionIndex = openTestCaseRunDescriptionsController.index(0,0);
                ui->runDescriptionTable->setCurrentIndex(firstRunDescriptionIndex);

                ui->fullFileNameBox->setText(dataModel.curItem->testCase->fullFileName);

                ui->currentConfigBox->clear();
                for(int i = 0; i < dataModel.curItem->testCase->configList.keys().count(); i++)
                {
                    ui->currentConfigBox->addItem(dataModel.curItem->testCase->configList.keys().at(i));
                }

                ui->currentConfigBox->setCurrentText(dataModel.curItem->testCase->currConfigName);
                ui->isLoadResultBox->setChecked(dataModel.curItem->isLoadResultsImediately);

                ui->descriptionCheckBox->setChecked(dataModel.curItem->isDescriptionFilter);
                ui->descriptionComboBox->setEditText(dataModel.curItem->descriptionFilter);
                ui->toCheckBox->setChecked(dataModel.curItem->isToDateTimeFilter);
                ui->toDateTimeEdit->setDateTime(dataModel.curItem->toDateTimeFilter);
                ui->fromCheckBox->setChecked(dataModel.curItem->isFromDateTimeFilter);
                ui->fromDateTimeEdit->setDateTime(dataModel.curItem->fromDateTimeFilter);

            } else {

                openTestCaseRunDescriptionsController.beginResetModel();
                openTestCaseRunDescriptionsController.dataSource = nullptr;
                openTestCaseRunDescriptionsController.endResetModel();

                ui->fullFileNameBox->clear();
                ui->currentConfigBox->clear();
                ui->currentConfigBox->clearEditText();
                ui->isLoadResultBox->setChecked(Qt::Unchecked);

                ui->descriptionCheckBox->setChecked(Qt::Unchecked);
                ui->descriptionComboBox->clear();
                ui->fromCheckBox->setChecked(Qt::Unchecked);
                ui->fromDateTimeEdit->clear();
                ui->toCheckBox->setChecked(Qt::Unchecked);
                ui->toDateTimeEdit->clear();
            }
        }
    }
}

void OpenTestCaseDialog::on_runDescriptionTableHeaderClicked(int arg1)
{
    if(arg1 == 0)
    {
        openTestCaseRunDescriptionsController.setAllChecked();
    }
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
        QModelIndex curIndex = ui->testCaseList->currentIndex();

        openTestCaseListController.beginResetModel();

        for(int i = 0; i < testCaseFileNames.count(); i++)
        {
            //Check if test-case already appended
            if(!dataModel.isTestCaseContained(testCaseFileNames.at(i)))
            {
                dataModel.addTestCase(testCaseFileNames.at(i));
            }
        }

        openTestCaseListController.endResetModel();

        if(!curIndex.isValid()) { curIndex = openTestCaseListController.index(0); }
        ui->testCaseList->setCurrentIndex(curIndex);
    }
}

void OpenTestCaseDialog::on_delBtn_clicked()
{
    int ind = ui->testCaseList->currentIndex().row();
    if(ind >= 0)
    {
        openTestCaseListController.beginResetModel();
        dataModel.removeTestCase(ind);
        openTestCaseListController.endResetModel();

        if(dataModel.itemList.length() == ind) { ind--; }

        if(ind >= 0)
        {
            QModelIndex index = openTestCaseListController.index(ind);
            ui->testCaseList->setCurrentIndex(index);
        }
        else
        {
            ui->fullFileNameBox->clear();
            ui->currentConfigBox->clear();
            ui->currentConfigBox->clearEditText();
            ui->isLoadResultBox->setChecked(Qt::Unchecked);
            dataModel.curItem = nullptr;
        }
    }
}

void OpenTestCaseDialog::on_currentConfigBox_currentIndexChanged(const QString &arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->testCase->currConfigName = arg1;
    }
}

void OpenTestCaseDialog::on_isLoadResultBox_stateChanged(int arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->isLoadResultsImediately = (ui->isLoadResultBox->checkState() == Qt::Checked);
    }
}

void OpenTestCaseDialog::on_descriptionCheckBox_stateChanged(int arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->isDescriptionFilter = ui->descriptionCheckBox->checkState() == Qt::Checked;
        ui->descriptionComboBox->setEnabled(dataModel.curItem->isDescriptionFilter);
    }
}

void OpenTestCaseDialog::on_fromCheckBox_stateChanged(int arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->isFromDateTimeFilter = ui->fromCheckBox->checkState() == Qt::Checked;
        ui->fromDateTimeEdit->setEnabled(dataModel.curItem->isFromDateTimeFilter);
    }
}

void OpenTestCaseDialog::on_toCheckBox_stateChanged(int arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->isToDateTimeFilter = ui->toCheckBox->checkState() == Qt::Checked;
        ui->toDateTimeEdit->setEnabled(dataModel.curItem->isToDateTimeFilter);
    }
}

void OpenTestCaseDialog::on_descriptionComboBox_currentTextChanged(const QString &arg1)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->descriptionFilter = arg1;
    }
}

void OpenTestCaseDialog::on_fromDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->fromDateTimeFilter = dateTime;
        if(ui->toDateTimeEdit->dateTime() < dateTime) {
            ui->toDateTimeEdit->setDateTime(dateTime);
        }
    }
}

void OpenTestCaseDialog::on_toDateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    if(dataModel.curItem != nullptr )
    {
        dataModel.curItem->toDateTimeFilter = dateTime;
        if(ui->fromDateTimeEdit->dateTime() > dateTime) {
            ui->fromDateTimeEdit->setDateTime(dateTime);
        }
    }
}

void OpenTestCaseDialog::on_setFilterBtn_clicked()
{
    openTestCaseRunDescriptionsController.beginResetModel();

    for(int i = 0; i < dataModel.itemList.count(); i++)
    {
        OpenTestCaseItem *item = dataModel.itemList.at(i);
        item->visibleRunDescriptions.clear();
        for(int j = 0; j < item->fullRunDescriptions.count(); j++)
        {
            OpenTestCaseRunItem *runItem = item->fullRunDescriptions.values().at(j);

            if(((ui->descriptionCheckBox->checkState() != Qt::Checked) ||
               (runItem->runDescription->Comment.contains(ui->descriptionComboBox->currentText()))) &&
               ((ui->fromCheckBox->checkState() != Qt::Checked) ||
               (runItem->runDescription->LocalDateTimeOfStart >= ui->fromDateTimeEdit->dateTime())) &&
               ((ui->toCheckBox->checkState() != Qt::Checked) ||
               (runItem->runDescription->LocalDateTimeOfStart <= ui->toDateTimeEdit->dateTime())))
            {
                item->visibleRunDescriptions.insert(runItem->runDescription->Num, runItem);
            }
        }

    }

    openTestCaseRunDescriptionsController.endResetModel();
}

void OpenTestCaseDialog::on_clearFilterBtn_clicked()
{
    ui->descriptionCheckBox->setCheckState(Qt::Unchecked);
    ui->fromCheckBox->setCheckState(Qt::Unchecked);
    ui->toCheckBox->setCheckState(Qt::Unchecked);

    openTestCaseRunDescriptionsController.beginResetModel();

    for(int i = 0; i < dataModel.itemList.count(); i++)
    {
        OpenTestCaseItem *item = dataModel.itemList.at(i);
        item->visibleRunDescriptions.clear();
        for(int j = 0; j < item->fullRunDescriptions.count(); j++)
        {
            int key = item->fullRunDescriptions.keys().at(j);
            item->visibleRunDescriptions.insert(key, item->fullRunDescriptions.value(key));
        }

    }

    openTestCaseRunDescriptionsController.endResetModel();
}
