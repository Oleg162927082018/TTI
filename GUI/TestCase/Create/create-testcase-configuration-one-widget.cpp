#include "create-testcase-configuration-one-widget.h"
#include "ui_create-testcase-configuration-one-widget.h"

CreateTestCaseConfigurationOneWidget::CreateTestCaseConfigurationOneWidget(ITestCaseEditWidget *extraWidget, QWidget *parent) :
    QWidget(parent), ui(new Ui::CreateTestCaseConfigurationOneWidget)
{
    ui->setupUi(this);

    extraWidget->setParent(this);
    extraWidget->setGeometry(0,0,500,250);
    this->ExtraWidget = extraWidget;
}

CreateTestCaseConfigurationOneWidget::~CreateTestCaseConfigurationOneWidget()
{
    delete ui;
}

void CreateTestCaseConfigurationOneWidget::GetCommonParams(TestCaseConfig &tc_cfg)
{
    tc_cfg.WaitingTime = ui->watingBox->value();
    tc_cfg.MaxThreads = ui->threadsBox->value();
    tc_cfg.CompressionLevel = ui->compressionBox->currentIndex();
    tc_cfg.isSaveOutput = ui->isSaveOutPutBox->checkState() == Qt::Checked;
    tc_cfg.description = ui->descriptionBox->toPlainText();
}

void CreateTestCaseConfigurationOneWidget::GetExtraParams(QDomDocument *config, QDomDocument *params)
{
    ExtraWidget->GetData(config, params);
}

QMap<QString, QDomDocument *> *CreateTestCaseConfigurationOneWidget::GetTestList()
{
    return ExtraWidget->GetTestList();
}
