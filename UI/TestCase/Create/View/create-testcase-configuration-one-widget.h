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
    explicit CreateTestCaseConfigurationOneWidget(ITestCaseEditWidget *extraWidget, QWidget *parent = nullptr);
    ~CreateTestCaseConfigurationOneWidget();

    ITestCaseEditWidget *ExtraWidget = nullptr;

    void GetCommonParams(TestCaseConfig &tc_cfg);
    void GetExtraParams(QString &config, QString &params);
    QMap<QString, QString> *GetTestList();

public:
    Ui::CreateTestCaseConfigurationOneWidget *ui;
};

#endif // CREATETESTCASEWIDGET_H
