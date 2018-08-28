#ifndef TESTRUNNABLECLASS_H
#define TESTRUNNABLECLASS_H

#include "Common/itestcasetemplate.h"
#include "Common/DBManager/test-result.h"

#include <QObject>
#include <QProcess>
#include <QRunnable>
#include <QColor>
#include <QVector>

struct TestInfo
{
    //Input
    QString testName;
    QString exeCommand;
    QStringList argumentList;
    QString workingFullFolderName;

    QString outputFullFolderName;
    QString benchmarkOutputFullFolderName;
    QString previousOutputFullFolderName; //Output of previous run it is any output wich was equal previous output!
    QStringList otherOutputFullFolderNames;

    int waitingTime;
    int compressionLevel;
    bool isSaveOutput;

    TestResult testResult;
    QStringList consoleOutput;

    ITestOutputComparator *comparator;
};

class TestExecutor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit TestExecutor(TestInfo *info);

signals:
    void finished(TestInfo *info);

public slots:
    void doKill();

public:
    void run();

private:
    QProcess *proc;
    TestInfo *info;

    bool CompressWithBenchmark();
    bool CompressWithPrevious();
    bool CompressWithOther();
};

#endif // TESTRUNNABLECLASS_H
