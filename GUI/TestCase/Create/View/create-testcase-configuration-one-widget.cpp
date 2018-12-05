#include "create-testcase-configuration-one-widget.h"
#include "ui_create-testcase-configuration-one-widget.h"

CreateTestCaseConfigurationOneWidget::CreateTestCaseConfigurationOneWidget(ITestCaseEditWidget *extraWidget, QWidget *parent) :
    QWidget(parent), ui(new Ui::CreateTestCaseConfigurationOneWidget)
{
    ui->setupUi(this);
    ui->paramSplitter->insertWidget(0, extraWidget);
    //extraWidget->setParent(this);
    //extraWidget->setGeometry(0,0,500,250);
    this->ExtraWidget = extraWidget;
}

CreateTestCaseConfigurationOneWidget::~CreateTestCaseConfigurationOneWidget()
{
    delete ui;
}

void CreateTestCaseConfigurationOneWidget::GetCommonParams(TestCaseConfig &tc_cfg)
{
    tc_cfg.waitingTime = ui->watingBox->value();
    tc_cfg.maxThreads = ui->threadsBox->value();
    tc_cfg.compressionLevel = ui->compressionBox->currentIndex();
    tc_cfg.isSaveOutput = ui->isSaveOutPutBox->checkState() == Qt::Checked;
    tc_cfg.description = ui->descriptionBox->toPlainText();
}

void CreateTestCaseConfigurationOneWidget::GetExtraParams(QString &config, QString &params)
{
    ExtraWidget->GetData(config, params);
}

QMap<QString, QString> *CreateTestCaseConfigurationOneWidget::GetTestList()
{
    return ExtraWidget->GetTestList();
}
