#include "set-filter-tag-folder-widget.h"
#include "ui_set-filter-tag-folder-widget.h"

SetFilterTagFolderWidget::SetFilterTagFolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFilterTagFolderWidget)
{
    ui->setupUi(this);
}

SetFilterTagFolderWidget::~SetFilterTagFolderWidget()
{
    delete ui;
}

void SetFilterTagFolderWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
