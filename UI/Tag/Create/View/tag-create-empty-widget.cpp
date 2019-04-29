#include "tag-create-empty-widget.h"
#include "ui_tag-create-empty-widget.h"

TagCreateEmptyWidget::TagCreateEmptyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagCreateEmptyWidget)
{
    ui->setupUi(this);
}

TagCreateEmptyWidget::~TagCreateEmptyWidget()
{
    delete ui;
}
