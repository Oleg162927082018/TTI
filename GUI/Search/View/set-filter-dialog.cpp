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

bool SetFilterDialog::isCondition(bool checked, QString name)
{
    bool condition = true;

    if(ui->markCheckBox->checkState() == Qt::Checked) {
        if(checked == (ui->markComboBox->currentText().compare("checked") == 0)) {
            condition = true;
        } else {
            condition = false;
        }
    }

    if(ui->nameCheckBox->checkState() == Qt::Checked) {
        if(name.contains(ui->nameComboBox->currentText())) {
            if((ui->nameOrAndBox->currentText().compare("OR") == 0)) {
                condition = true;
            }
        } else {
            if((ui->nameOrAndBox->currentText().compare("AND") == 0)) {
                condition = false;
            }
        }
    }

    return condition;
}
