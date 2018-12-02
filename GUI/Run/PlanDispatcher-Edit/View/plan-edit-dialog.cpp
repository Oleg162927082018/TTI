#include "plan-edit-dialog.h"
#include "ui_plan-edit-dialog.h"

PlanEditDialog::PlanEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanEditDialog)
{
    ui->setupUi(this);
}

PlanEditDialog::~PlanEditDialog()
{
    delete ui;
}
