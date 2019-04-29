#include "tag-edit-dialog.h"
#include "ui_tag-edit-dialog.h"

TagEditDialog::TagEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagEditDialog)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags &= ~Qt::WindowContextHelpButtonHint;
    setWindowFlags(flags);
}

TagEditDialog::~TagEditDialog()
{
    delete ui;
}
