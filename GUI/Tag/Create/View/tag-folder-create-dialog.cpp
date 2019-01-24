#include "tag-folder-create-dialog.h"
#include "ui_tag-folder-create-dialog.h"

TagFolderCreateDialog::TagFolderCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagFolderCreateDialog)
{
    ui->setupUi(this);
}

TagFolderCreateDialog::~TagFolderCreateDialog()
{
    delete ui;
}

QString TagFolderCreateDialog::folderName()
{
    return ui->folderNameBox->text();
}

QString TagFolderCreateDialog::folderDescription()
{
    return ui->folderDescriptionBox->toPlainText();
}
