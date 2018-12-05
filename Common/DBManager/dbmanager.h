#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "../itestcasetemplate.h"

#include "test-case.h"
#include "test-status.h"
#include "run-description.h"
#include "test-result.h"
#include "tag-collection.h"

class DBManager
{
public:
    // Full file name creation tools
    static QString GetRunName(int runNum);
    static QString GetOutFolder(QString testCaseFullFileName, QString testName, QString RunName);
    static QString GetResultFullFileName(QString testCaseFullFileName, QString RunName, QString testName, QString ext);
    static QString GetStatusFullFileName(QString testCaseFullFileName, QString testName, QString ext);

    // Test-case
    static void SaveTestCase(QString testCaseFullFileName, TestCase *tc);
    static TestCase *GetTestCase(QString testCaseFullFileName);

    // Test status
    //Create folders and status files. testList must contain pairs path to status file and content of <data> section
    //which needed for run test. Example: <"srcFolder/subFolder/subSubFolder/test-001.script", "<path src="0">test-001.script</path>">
    static void CreateTestCaseInfrastructure(QString testCaseFullFileName, QMap<QString, QString> *testList);
    static QStringList GetTestCaseFolders(QString testCaseFullFileName, QString relativeOwnerFolderName);
    static QList<TestStatus *> GetTestCaseFolderItems(QString testCaseFullFileName, QString relativeOwnerFolderName);
    static TestStatus *GetTestStatus(QString testCaseFullFileName, QString statusRelativeFileName);
    static void SaveTestStatus(QString testCaseFullFileName, TestStatus *ts);

    // Test-plan
    static void SaveTestingPlan(QString planFullFileName,QList<TestCase*> *testCases, QList<QPair<TestCase *, TestStatus *>> *tests, bool isRunOnce, bool isUpdateLastResult);
    static QDomDocument *GetTestingPlan(QString planFullFileName);

    // Run description
    static void SaveRunDescription(QString testCaseFullFileName, QString runName, RunDescription *rd);
    static RunDescription *GetRunDescription(QString testCaseFullFileName, QString runName);

    // Run result
    static void SaveTestResult(QString testCaseFullFileName, QString runName, QString relativeTestName, TestResult *tr);
    static void SaveTestConsoleOutput(QString testCaseFullFileName, QString runName, QString relativeTestName, QStringList *consoleOutput);
    static TestResult *GetTestResult(QString testCaseFullFileName, QString relativeTestName, QString runName);
    static QByteArray GetConsoleLog(QString testCaseFullFileName, QString relativeTestName, QString runName);

    //Other
    static QVector<int> *GetRunList(QString testCaseFullFileName);
    static QVector<int> *GetOutList(QString testCaseFullFileName, QString relativeTestName);

    //Tags
    static void SaveTagCollection(TagItem *tagCollection);
    static void SaveTagFolderElements(QDomElement &folderElement, TagItem *tagItem);
    static TagItem *GetTagCollection(QString fullFileName);
    static void LoadTagFolderElements(QDomElement &folderElement, TagItem *tagFolder);
    static void DeleteTagCollection(QString fullFileName);
};

#endif // DBMANAGER_H
