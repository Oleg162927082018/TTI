#include "rundescription-show-dialog.h"
#include "ui_rundescription-show-dialog.h"

RunDescriptionShowDialog::RunDescriptionShowDialog(QMap<int, RunDescription *> *runDescriptionList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RunDescriptionShowDialog)
{
    ui->setupUi(this);

    runDescriptionTableWidget = new RunDescriptionTableWidget(this);
    runDescriptionTableWidget->SetDataSource(runDescriptionList);
    ui->verticalLayout->insertWidget(0, runDescriptionTableWidget);
}

RunDescriptionShowDialog::~RunDescriptionShowDialog()
{
    delete ui;
}
