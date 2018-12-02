#include "create-testcase-configuration-list-widget.h"
#include "ui_create-testcase-configuration-list-widget.h"

#include "Common/DllManager/dllmanager.h"
#include "Common/DBManager/dbmanager.h"

#include "create-testcase-configuration-one-widget.h"

#include <QMessageBox>
#include <QInputDialog>

CreateTestCaseConfigurationListWidget::CreateTestCaseConfigurationListWidget(QString id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateTestCaseConfigurationListWidget)
{
    ui->setupUi(this);

    ID = id;
}

CreateTestCaseConfigurationListWidget::~CreateTestCaseConfigurationListWidget()
{
    delete ui;
}

bool CreateTestCaseConfigurationListWidget::SaveTestCase(QString fullFileName)
{
    if(ui->configTabs->count() == 0)
    {
        QMessageBox mb("Error",
                       "You need at least one configuration!",
                       QMessageBox::Critical,
                       QMessageBox::Ok | QMessageBox::Default,
                       QMessageBox::NoButton,
                       QMessageBox::NoButton);
        mb.exec();

        return false;
    }

    //Save Test-Case file
    TestCase tc;
    tc.ID = ID;
    tc.currConfigName = ui->currentConfigBox->currentText();

    for(int i =0; i < ui->currentConfigBox->count(); i++)
    {
        QWidget *widget = ui->configTabs->widget(i);
        CreateTestCaseConfigurationOneWidget *subWidget = static_cast<CreateTestCaseConfigurationOneWidget *>(widget);

        QString cfg;
        TestCaseConfig tc_cfg;

        subWidget->GetCommonParams(tc_cfg);
        subWidget->GetExtraParams(cfg, tc_cfg.extraParams);

        tc.configList.insert(ui->currentConfigBox->itemText(i), tc_cfg);
    }

    DBManager::SaveTestCase(fullFileName, &tc);

    //Save test-case file cache
    int currrentConfigIndex = ui->currentConfigBox->currentIndex();
    QWidget *widget = ui->configTabs->widget(currrentConfigIndex);
    CreateTestCaseConfigurationOneWidget *currrentConfigWidget = static_cast<CreateTestCaseConfigurationOneWidget *>(widget);
    QMap<QString, QString> *testList = currrentConfigWidget->GetTestList();
    DBManager::CreateTestCaseInfrastructure(fullFileName, testList);
    delete testList;


    return true;
}

void CreateTestCaseConfigurationListWidget::on_newConfigurationButton_clicked()
{
    bool ok;
    QString newCfgName = QInputDialog::getText(this, "Input configuration name.",
        "Configuration name:", QLineEdit::Normal, "", &ok);

    if (ok && !newCfgName.isEmpty())
    {
        for(int i = 0; i < ui->configTabs->tabBar()->count(); i++)
        {
            if(ui->configTabs->tabText(i).compare(newCfgName) == 0)
            {
                QMessageBox mb("Error",
                               "Configuration name must be unique!",
                               QMessageBox::Critical,
                               QMessageBox::Ok | QMessageBox::Default,
                               QMessageBox::NoButton,
                               QMessageBox::NoButton);
                mb.exec();
                return;
            }
        }

        ui->currentConfigBox->addItem(newCfgName);

        CreateTestCaseConfigurationOneWidget *templateParamWidget =
                new CreateTestCaseConfigurationOneWidget(DLLManager::GetTestCaseEditWidget(ID, this), this);
        int newTabIndex = ui->configTabs->addTab(templateParamWidget, newCfgName);
        ui->configTabs->setCurrentIndex(newTabIndex);
    }
}

void CreateTestCaseConfigurationListWidget::on_configTabs_tabCloseRequested(int index)
{
    ui->currentConfigBox->removeItem(index);

    QWidget *w = ui->configTabs->widget(index);
    ui->configTabs->removeTab(index);
    delete w;
}
