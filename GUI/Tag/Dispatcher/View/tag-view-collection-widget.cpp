#include "tag-view-collection-widget.h"
#include "ui_tag-view-collection-widget.h"

TagViewCollectionWidget::TagViewCollectionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagViewCollectionWidget)
{
    ui->setupUi(this);
}

TagViewCollectionWidget::~TagViewCollectionWidget()
{
    delete ui;
}

void TagViewCollectionWidget::setData(QString description)
{
    ui->descEdit->setHtml(description);
}
