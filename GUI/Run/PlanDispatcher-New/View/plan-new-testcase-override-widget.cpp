#include "plan-new-testcase-override-widget.h"
#include "ui_plan-new-testcase-override-widget.h"

PlanNewTestCaseOverrideWidget::PlanNewTestCaseOverrideWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlanNewTestCaseOverrideWidget)
{
    ui->setupUi(this);
}

PlanNewTestCaseOverrideWidget::~PlanNewTestCaseOverrideWidget()
{
    delete ui;
}
