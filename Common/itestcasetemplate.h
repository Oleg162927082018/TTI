#ifndef ITESTCASETEMPLATE_H
#define ITESTCASETEMPLATE_H

#include <QtXml/QDomDocument>
#include <QString>
#include <QWidget>
#include <QMap>
#include <QColor>
#include <QtCore/qglobal.h>

// Class comparator need because it send to another threads
class ITestOutputComparator
{
public:
    virtual ~ITestOutputComparator() { }

    //benchmarkCompareResult
    // == 0 if benchmark equal current result
    // > 0  if benchmark not equal current result, value is a metrics
    // == -1 if benchmark not found
    //previousCompareResult
    // == 0 if previous equal current result
    // > 0  if previous not equal current result, value is a metrics
    // == -1 if previous not found
    virtual void CalculateStatus(QStringList consoleOutput, QString outputFullFolderName,
                                 int exitCode, qint64 exeTimeMs, int benchmarkCompareResult, int previousCompareResult,
                                 QString &out_status, QColor &out_color) = 0;
    virtual int Compare(QString outputFullFolderName1, QString outputFullFolderName2) = 0;
};

class ITestCaseEditWidget : public QWidget
{
public:
    explicit ITestCaseEditWidget(QWidget *parent = 0) : QWidget(parent) { }
    virtual ~ITestCaseEditWidget() { }

    virtual void SetData(QDomDocument *config, QDomDocument *params) = 0;
    virtual void GetData(QDomDocument *config, QDomDocument *params) = 0;
    virtual QMap<QString, QDomDocument *> *GetTestList() = 0;

};

class IResultCompareWidget : public QWidget
{
public:
    explicit IResultCompareWidget(QWidget *parent = 0) : QWidget(parent) { }
    virtual ~IResultCompareWidget() { }

    virtual void SetLeftValue(QString leftValueFullFolderPath) = 0;
    virtual void SetRightValue(QString rightValueFullFolderPath) = 0;
};

class ITestCaseAdapter
{
public:
    virtual ~ITestCaseAdapter() { }

    virtual const QString ID() = 0;
    virtual const QString Name() = 0;
    virtual const QString Description() = 0;

    virtual ITestCaseEditWidget *GetTestCaseEditWidget(QWidget *parent = 0) = 0;

    virtual void GetRunCommand(QString testCaseFullFileName, QDomDocument extraParams, QDomDocument testParams, QString outputFullFolderName,
                               QString &out_cmd, QStringList &out_arg, QString &out_workDir) = 0;

    virtual ITestOutputComparator *GetComparator() = 0;
    virtual IResultCompareWidget *GetResultCompareWidget(QWidget *parent = 0) = 0;
};

extern "C" Q_DECL_EXPORT QList<ITestCaseAdapter *> GetTestCaseAdapters(void);

//--------------------------------------------------------------

#define GetTestCaseAdaptersFunctionName "GetTestCaseAdapters"

typedef QList<ITestCaseAdapter *> (*GetTestCaseAdaptersFunction)();

#endif // ITESTCASETEMPLATE_H
