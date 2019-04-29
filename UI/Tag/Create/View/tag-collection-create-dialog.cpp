#include "tag-collection-create-dialog.h"
#include "ui_tag-collection-create-dialog.h"

#include <QFileDialog>

TagCollectionCreateDialog::TagCollectionCreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagCollectionCreateDialog)
{
    ui->setupUi(this);
}

TagCollectionCreateDialog::~TagCollectionCreateDialog()
{
    delete ui;
}

QString TagCollectionCreateDialog::collectionFileName()
{
    return ui->tagCollectionNameBox->text();
}

QString TagCollectionCreateDialog::collectionDescription()
{
    return ui->tagCollectionDescriptionBox->toPlainText();
}

void TagCollectionCreateDialog::on_fileNameBtn_clicked()
{
    //Get file name for tag collection
    QString fn = QFileDialog::getSaveFileName(this, "New Tag collection",
        QDir::cleanPath(qApp->applicationDirPath() + "/tags"), "Tag collection (*.xml)");

    if(!fn.isEmpty()) {
        ui->tagCollectionNameBox->setText(fn);
    }
}
