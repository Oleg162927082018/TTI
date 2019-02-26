#ifndef RUNPLANMANAGER_H
#define RUNPLANMANAGER_H

#include "testexecutor.h"

#include <QObject>
#include <QStringList>
#include <QtXml/QDomDocument>
#include <QDateTime>
#include <QMap>

enum PlanStatus { Wating = 0, Prosessing = 1, Finished = 2, Pause = 3 };

struct PlanQueueItem
{
    QString fullFileName;
    QString description;

    bool isUpdateLastResult;
    bool isDeleteAfterRun;

    PlanStatus status;

    QStringList log;
    int testCount = 0;
    int testFinished = 0;

    //Buffered value for current run.
    //Xml document buffer
    QDomDocument *planXml;
    QDomElement  processedTestCase;
    QDomElement  processedTest;

    //Current test-case values
    QString processedTestCaseExtraParams;

    QString processedTestCaseID;
    QString processedTestCaseFullFileName;

    int processedTestCaseWaitingTime;
    int processedTestCaseCompressionLevel;
    int processedTestCaseMaxThreads;
    bool processedTestCaseIsSaveOutPut;

    QString processedTestCaseRunName;

    //Current test-case statistics
    QDateTime processedTestCaseStart;
    int processedTestCaseMinTreads = 0; //Must changed if currentMaxThreads changed outside
    int processedTestCaseMaxTreads = 0; //Now not implemented
    QMap<QString, int> processedTestCaseStatistics;
};

class RunManager : public QObject
{
    Q_OBJECT

public:
    static RunManager *Handle;

    static void Init();
    static void FreeResources();

    static int GetQueueLength();
    static void AddPlan(QString planFullFileName, QString runDescription);
    static PlanQueueItem *GetPlan(int ind);
    static void RemovePlan(int ind);
    static void MoveUp(int ind);
    static void MoveDown(int ind);

    static void Start(int ind);
    static void Stop();
    static void Pause();

signals:
    void logChanged(int planInd, int newLine);

    void testStarted(int planInd);
    void testFinished(int planInd);

    void testCaseStarted(int planInd);
    void testCaseFinished(int planInd);

    void planStarted(int planInd);
    void planFinished(int planInd);

    void allFinished();

    void killTests();

private slots:
    void onTestFinished(TestInfo *info);

private:
    explicit RunManager(QObject *parent = nullptr);
    ~RunManager();

    QList<PlanQueueItem *> planQueue;
    int processingPlan = -1;

    int lookingForPlan(int planInd);
    void doPlan(PlanQueueItem *plan);

    void beginWatingPlan(PlanQueueItem *plan);
    void beginPausePlan(PlanQueueItem *plan);
    void endPlan(PlanQueueItem *plan);

    void beginNextTestCase(PlanQueueItem *plan);
    void cacheTestCaseData(PlanQueueItem *plan);
    void endTestCase(PlanQueueItem *plan);

    void beginTest(PlanQueueItem *plan);
    void endTest(TestInfo *testInfo, PlanQueueItem *plan);

    void beginQueue();
    void endQueue();

private:
    int processingTestCount = 0;
};

#endif // RUNPLANMANAGER_H
