#include <QHelpEngine>

#include "helpmanager.h"

HelpDialog *HelpManager::viewer = nullptr;

void HelpManager::Init()
{
    QString collectionFile = "help/tti.qhc";

    viewer = new HelpDialog(new QHelpEngine(collectionFile));
}

void HelpManager::showIndex()
{
    if (viewer != nullptr)
    {
        viewer->showHelp(1);
    }
}

void HelpManager::showContents()
{
    if (viewer != nullptr)
    {
        viewer->showHelp(0);
    }
}

void HelpManager::FreeResources()
{
    delete viewer;
    viewer = nullptr;
}
