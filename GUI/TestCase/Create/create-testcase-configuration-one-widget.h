#ifndef CREATETESTCASEWIDGET_H
#define CREATETESTCASEWIDGET_H

#include "Common/DBManager/dbmanager.h"

#include <QWidget>
#include <QDomDocument>

namespace Ui {
class CreateTestCaseConfigurationOneWidget;
}

class CreateTestCaseConfigurationOneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CreateTestCaseConfigurationOneWidget(ITestCaseEditWidget *extraWidget, QWidget *parent = 0);
    ~CreateTestCaseConfigurationOneWidget();

    ITestCaseEditWidget *ExtraWidget = NULL;

    void GetCommonParams(TestCaseConfig &tc_cfg);
    void GetExtraParams(QDomDocument *config, QDomDocument *params);
    QMap<QString, QDomDocument *> *GetTestList();

public:
    Ui::CreateTestCaseConfigurationOneWidget *ui;
};

#endif // CREATETESTCASEWIDGET_H
