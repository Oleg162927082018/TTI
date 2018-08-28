#include "tag-view-folder-widget.h"
#include "ui_tag-view-folder-widget.h"

TagViewFolderWidget::TagViewFolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagViewFolderWidget)
{
    ui->setupUi(this);
}

TagViewFolderWidget::~TagViewFolderWidget()
{
    delete ui;
}

void TagViewFolderWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
