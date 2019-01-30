#include "tag-dispatcher-unknown-widget.h"
#include "ui_tag-dispatcher-unknown-widget.h"

TagDispatcherUnknownWidget::TagDispatcherUnknownWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TagDispatcherUnknownWidget)
{
    ui->setupUi(this);
}

TagDispatcherUnknownWidget::~TagDispatcherUnknownWidget()
{
    delete ui;
}
