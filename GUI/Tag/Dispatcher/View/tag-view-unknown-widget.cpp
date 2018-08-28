#include "tag-view-unknown-widget.h"
#include "ui_tag-view-unknown-widget.h"

TagViewUnknownWidget::TagViewUnknownWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagViewUnknownWidget)
{
    ui->setupUi(this);
}

TagViewUnknownWidget::~TagViewUnknownWidget()
{
    delete ui;
}
