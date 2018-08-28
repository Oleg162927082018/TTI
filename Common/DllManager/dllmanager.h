#ifndef DLLMANAGER_H
#define DLLMANAGER_H

#include "Common/itestcasetemplate.h"

class DLLManager
{
public:
    static void Init();
    static void FreeResources();

    static QStringList GetTestCaseAdapterIDList();
    static ITestCaseAdapter *GetTestCaseAdapter(QString id);

    static ITestCaseEditWidget *GetTestCaseEditWidget(QString id, QWidget *parent = 0);
    static IResultCompareWidget *GetTestCaseResultCompareWidget(QString id, QWidget *parent = 0);

    static void GetTestCaseRunCommand(QString id, QString testCaseFullFileName, QDomDocument extraParams, QDomDocument testParams,
                                      QString outputFullFolderName, QString &out_cmd, QStringList &out_arg, QString &out_workDir);

    static ITestOutputComparator *GetTestCaseComparator(QString id);

private:
    static QHash<QString, ITestCaseAdapter *> testCaseCollection;
};

#endif // DLLMANAGER_H
