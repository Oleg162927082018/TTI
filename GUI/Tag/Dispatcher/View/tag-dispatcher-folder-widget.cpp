#include "tag-dispatcher-folder-widget.h"
#include "ui_tag-view-folder-widget.h"

TagDispatcherFolderWidget::TagDispatcherFolderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDispatcherFolderWidget)
{
    ui->setupUi(this);
}

TagDispatcherFolderWidget::~TagDispatcherFolderWidget()
{
    delete ui;
}

void TagDispatcherFolderWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
