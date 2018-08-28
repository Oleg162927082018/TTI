#include <QHelpEngine>

#include "helpmanager.h"

HelpDialog *HelpManager::viewer = 0;

void HelpManager::Init()
{
    QString collectionFile = "help/tti.qhc";

    viewer = new HelpDialog(new QHelpEngine(collectionFile));
}

void HelpManager::showIndex()
{
    if (viewer != 0)
    {
        viewer->showHelp(1);
    }
}

void HelpManager::showContents()
{
    if (viewer != 0)
    {
        viewer->showHelp(0);
    }
}

void HelpManager::FreeResources()
{
    delete viewer;
    viewer = 0;
}
