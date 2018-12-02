#include "set-filter-dialog.h"
#include "ui_set-filter-dialog.h"

SetFilterDialog::SetFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetFilterDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);
}

SetFilterDialog::~SetFilterDialog()
{
    delete ui;
}
