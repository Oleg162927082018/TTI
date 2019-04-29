#include "set-filter-tag-unknown-widget.h"
#include "ui_set-filter-tag-unknown-widget.h"

SetFilterTagUnknownWidget::SetFilterTagUnknownWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetFilterTagUnknownWidget)
{
    ui->setupUi(this);
}

SetFilterTagUnknownWidget::~SetFilterTagUnknownWidget()
{
    delete ui;
}
