#include "run-headers-dialog.h"
#include "ui_run-headers-dialog.h"

RunHeadersDialog::RunHeadersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunHeadersDialog)
{
    ui->setupUi(this);

    runHeadersTableController.setDataSource(&visibleHeadersData);
    ui->runDescriptionTable->setModel(&runHeadersTableController);

    connect(ui->runDescriptionTable->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(on_runDescriptionTableHeaderClicked(int)));

}

RunHeadersDialog::~RunHeadersDialog()
{
    delete ui;

    for(int i = 0; i < fullHeadersData.count(); i++ ) { delete fullHeadersData.values().at(i); }
}

void RunHeadersDialog::addRunDescription(QString name, bool visible, RunDescription *rd)
{
    runHeadersTableController.beginResetModel();

    RunHeaderItem *item = new RunHeaderItem();
    item->rd = rd;
    item->checked = visible;
    item->name = name;

    fullHeadersData.insert(rd->Num, item);
    visibleHeadersData.insert(rd->Num, item);

    runHeadersTableController.endResetModel();
}

QMap<int, RunHeaderItem *> *RunHeadersDialog::getRunDescriptions()
{
    return &fullHeadersData;
}

void RunHeadersDialog::on_descriptionCheckBox_stateChanged(int arg1)
{
    ui->descriptionComboBox->setEnabled(arg1 == Qt::Checked);
}

void RunHeadersDialog::on_fromCheckBox_stateChanged(int arg1)
{
    ui->fromDateTimeEdit->setEnabled(arg1 == Qt::Checked);
}

void RunHeadersDialog::on_toCheckBox_stateChanged(int arg1)
{
    ui->toDateTimeEdit->setEnabled(arg1 == Qt::Checked);
}

void RunHeadersDialog::on_setFilterBtn_clicked()
{
    runHeadersTableController.beginResetModel();

    visibleHeadersData.clear();
    for(int i = 0; i < fullHeadersData.count(); i++)
    {
        RunHeaderItem *item = fullHeadersData.values().at(i);
        if(((ui->descriptionCheckBox->checkState() != Qt::Checked) ||
           (item->rd->Comment.contains(ui->descriptionComboBox->currentText()))) &&
           ((ui->fromCheckBox->checkState() != Qt::Checked) ||
           (item->rd->LocalDateTimeOfStart >= ui->fromDateTimeEdit->dateTime())) &&
           ((ui->toCheckBox->checkState() != Qt::Checked) ||
           (item->rd->LocalDateTimeOfStart <= ui->toDateTimeEdit->dateTime())))
        {
            visibleHeadersData.insert(item->rd->Num, item);
        }
    }

    runHeadersTableController.endResetModel();
}

void RunHeadersDialog::on_clearFilterBtn_clicked()
{
    ui->descriptionCheckBox->setCheckState(Qt::Unchecked);
    ui->fromCheckBox->setCheckState(Qt::Unchecked);
    ui->toCheckBox->setCheckState(Qt::Unchecked);

    runHeadersTableController.beginResetModel();

    for(int j = 0; j < fullHeadersData.count(); j++)
    {
        int key = fullHeadersData.keys().at(j);
        visibleHeadersData.insert(key, fullHeadersData.value(key));
    }

    runHeadersTableController.endResetModel();
}

void RunHeadersDialog::on_runDescriptionTableHeaderClicked(int arg1)
{
    if(arg1 == 0)
    {
        runHeadersTableController.setAllChecked();
    }
}
