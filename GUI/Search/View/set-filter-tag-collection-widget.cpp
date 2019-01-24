#include "set-filter-tag-collection-widget.h"
#include "ui_set-filter-tag-collection-widget.h"

SetFilterTagCollectionWidget::SetFilterTagCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFilterTagCollectionWidget)
{
    ui->setupUi(this);
}

SetFilterTagCollectionWidget::~SetFilterTagCollectionWidget()
{
    delete ui;
}

void SetFilterTagCollectionWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
