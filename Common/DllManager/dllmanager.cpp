#include "dllmanager.h"

#include <QDir>
#include <QLibrary>
#include <QApplication>

//-- Variables -------------------------------------
QHash<QString, ITestCaseAdapter *> DLLManager::testCaseCollection;


//-- Methods ---------------------------------------

void DLLManager::Init()
{
    //Clear test-case collection
    DLLManager::FreeResources();

    //Looking for and load test-cases from dll
    QDir appDir(QDir::cleanPath(qApp->applicationDirPath() + "/lib"));
    appDir.setFilter(QDir::Files);
    appDir.setNameFilters(QStringList("*.dll"));
    appDir.setSorting(QDir::Name);

    QFileInfoList testCaseLibList = appDir.entryInfoList();
    for (int i = 0; i < testCaseLibList.size(); ++i)
    {
        QFileInfo fileInfo = testCaseLibList.at(i);
        QLibrary testCaseLib(fileInfo.absoluteFilePath());
        if (testCaseLib.load())
        {
            GetTestCaseAdaptersFunction GetTestCaseList =
                    (GetTestCaseAdaptersFunction)testCaseLib.resolve(GetTestCaseAdaptersFunctionName);
            {
                QList<ITestCaseAdapter *> tcList = GetTestCaseList();
                for(int j = 0; j < tcList.size(); j++)
                {
                    ITestCaseAdapter* tc = tcList[j];
                    DLLManager::testCaseCollection.insert(tc->ID(), tc);
                }
            }
        }
    }
}

void DLLManager::FreeResources()
{
    foreach(ITestCaseAdapter *tc, DLLManager::testCaseCollection) { delete tc; }
}

QStringList DLLManager::GetTestCaseAdapterIDList()
{
    return DLLManager::testCaseCollection.keys();
}

ITestCaseAdapter *DLLManager::GetTestCaseAdapter(QString id)
{
    return DLLManager::testCaseCollection.value(id);
}

ITestCaseEditWidget *DLLManager::GetTestCaseEditWidget(QString id, QWidget *parent)
{
    return DLLManager::testCaseCollection.value(id)->GetTestCaseEditWidget(parent);
}

void DLLManager::GetTestCaseRunCommand(QString id, QString testCaseFullFileName, QDomDocument extraParams, QDomDocument testParams,
                                       QString outputFullFolderName, QString &out_cmd, QStringList &out_arg, QString &out_workDir)
{
    return DLLManager::testCaseCollection.value(id)->GetRunCommand(testCaseFullFileName, extraParams, testParams, outputFullFolderName, out_cmd, out_arg, out_workDir);
}

ITestOutputComparator *DLLManager::GetTestCaseComparator(QString id)
{
    return DLLManager::testCaseCollection.value(id)->GetComparator();
}

IResultCompareWidget *DLLManager::GetTestCaseResultCompareWidget(QString id, QWidget *parent)
{
    return DLLManager::testCaseCollection.value(id)->GetResultCompareWidget(parent);
}
