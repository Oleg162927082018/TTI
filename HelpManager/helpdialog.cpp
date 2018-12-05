#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QHelpSearchQueryWidget>
#include <QSplitter>
#include <QDockWidget>

#include "helpbrowser.h"

#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QHelpEngine *helpEng, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    helpEngine = helpEng;
    if(helpEngine!=nullptr)
    {
        if (helpEngine->setupData())
        {
            tabWidget = new QTabWidget;
            tabWidget->addTab(helpEngine->contentWidget(), "Contents");
            tabWidget->addTab(helpEngine->indexWidget(), "Index");

            textViewer = new HelpBrowser(helpEngine);
            textViewer->setSource(QUrl("qthelp://com.tti/TTI/about-tti/what-is-total-test-integrator.html"));

            QWidget *contentWidget = helpEngine->contentWidget();
            connect(contentWidget, SIGNAL(linkActivated(QUrl)), textViewer, SLOT(setSource(QUrl)));

            QWidget *indexWidget = helpEngine->indexWidget();
            connect(indexWidget, SIGNAL(linkActivated(QUrl, QString)), textViewer, SLOT(setSource(QUrl)));

            QSplitter *horizSplitter = new QSplitter(Qt::Horizontal);
            horizSplitter->insertWidget(0, tabWidget);
            horizSplitter->setStretchFactor(0,30);
            horizSplitter->insertWidget(1, textViewer);
            horizSplitter->setStretchFactor(1,70);
            ui->dialogLayout->addWidget(horizSplitter);
        }
        else
        {
            delete helpEngine;
            helpEngine = nullptr;
        }
    }
}

HelpDialog::~HelpDialog()
{
    delete ui;

    if (helpEngine != nullptr)
    {
        delete helpEngine;
        helpEngine = nullptr;
    }
}

void HelpDialog::showHelp(int activeTab)
{
    tabWidget->setCurrentIndex(activeTab);
    show();
}
