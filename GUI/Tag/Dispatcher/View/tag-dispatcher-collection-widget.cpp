#include "tag-dispatcher-collection-widget.h"
#include "ui_tag-view-collection-widget.h"

TagDispatcherCollectionWidget::TagDispatcherCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDispatcherCollectionWidget)
{
    ui->setupUi(this);
}

TagDispatcherCollectionWidget::~TagDispatcherCollectionWidget()
{
    delete ui;
}

void TagDispatcherCollectionWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
