#include "run-description-table-widget.h"
#include "ui_run-description-table-widget.h"

RunDescriptionTableWidget::RunDescriptionTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunDescriptionTableWidget)
{
    ui->setupUi(this);

    ui->runDescriptionTable->setModel(&runDescriptionTableController);
    ui->runDescriptionTable->setColumnWidth(0, 50);
    ui->runDescriptionTable->setColumnWidth(1, 140);
    ui->runDescriptionTable->setColumnWidth(2, 200);

    connect(ui->runDescriptionTable->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(on_runDescriptionTableHeaderClicked(int)));
}

RunDescriptionTableWidget::~RunDescriptionTableWidget()
{
    delete ui;
}

void RunDescriptionTableWidget::SetDataSource(QMap<int, RunDescription *> *runDescriptionList)
{
    runDescriptionTableController.setDataSource(runDescriptionList);
}

void RunDescriptionTableWidget::on_runDescriptionTableHeaderClicked(int arg1)
{
    if(arg1 == 0)
    {
        runDescriptionTableController.setAllChecked();
    }
}
