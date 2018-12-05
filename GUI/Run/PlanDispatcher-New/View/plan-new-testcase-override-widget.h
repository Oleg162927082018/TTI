#ifndef PLANNEWTESTCASEOVERRIDEWIDGET_H
#define PLANNEWTESTCASEOVERRIDEWIDGET_H

#include <QWidget>

namespace Ui {
class PlanNewTestCaseOverrideWidget;
}

class PlanNewTestCaseOverrideWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlanNewTestCaseOverrideWidget(QWidget *parent = nullptr);
    ~PlanNewTestCaseOverrideWidget();

private:
    Ui::PlanNewTestCaseOverrideWidget *ui;
};

#endif // PLANNEWTESTCASEOVERRIDEWIDGET_H
