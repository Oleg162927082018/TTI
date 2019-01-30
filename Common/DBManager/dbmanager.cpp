#include "dbmanager.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <../TTI/Common/itestcasetemplate.h>

QString DBManager::GetRunName(int runNum)
{
    return QString("%1").arg(runNum, 8, 10,  QLatin1Char('0'));
}

QString DBManager::GetOutFolder(QString testCaseFullFileName, QString testName, QString RunName)
{
    return QDir::cleanPath(testCaseFullFileName.left(testCaseFullFileName.indexOf(".")) + "/OUTPUT/" + testName + "/" + RunName);
}

QString DBManager::GetResultFullFileName(QString testCaseFullFileName, QString RunName, QString testName, QString ext)
{
    return QDir::cleanPath(testCaseFullFileName.left(testCaseFullFileName.indexOf(".")) + "/RESULT/"+ RunName + "/" + testName + "." + ext);
}

QString DBManager::GetStatusFullFileName(QString testCaseFullFileName, QString testName, QString ext)
{
    return QDir::cleanPath(testCaseFullFileName.left(testCaseFullFileName.indexOf(".")) + "/STATUS/"+ testName + "." + ext);
}

void DBManager::SaveTestCase(QString testCaseFullFileName, TestCase *tc)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("test-case");
    doc.appendChild(rootNode);

    //ID
    QDomElement idElement = doc.createElement("ID");
    rootNode.appendChild(idElement);

    QDomText idValue = doc.createTextNode(tc->ID);
    idElement.appendChild(idValue);

    //Config list
    QDomElement configListNode = doc.createElement("config-list");
    configListNode.setAttribute("current", tc->currConfigName);
    rootNode.appendChild(configListNode);

    for(int i = 0; i < tc->configList.count(); i++)
    {
        QString name = tc->configList.keys().at(i);
        TestCaseConfig tc_cfg = tc->configList.value(name);

        QDomElement configNode = doc.createElement("config");
        configNode.setAttribute("name", name);

        //Exit Wating Time
        QDomElement watingElement = doc.createElement("waiting-sec");
        configNode.appendChild(watingElement);

        QDomText watingValue = doc.createTextNode(QString::number(tc_cfg.waitingTime));
        watingElement.appendChild(watingValue);

        //Max Threads
        QDomElement threadsElement = doc.createElement("max-threads");
        configNode.appendChild(threadsElement);

        QDomText threadsValue = doc.createTextNode(QString::number(tc_cfg.maxThreads));
        threadsElement.appendChild(threadsValue);

        //Compression Level
        QDomElement compressionElement = doc.createElement("compression");
        configNode.appendChild(compressionElement);

        QDomText compressionValue = doc.createTextNode(QString::number(tc_cfg.compressionLevel));
        compressionElement.appendChild(compressionValue);

        //Console output
        QString isSaveOutput = tc_cfg.isSaveOutput?"true":"false";

        QDomElement outputElement = doc.createElement("is-save-output");
        configNode.appendChild(outputElement);

        QDomText isSaveOutputValue = doc.createTextNode(isSaveOutput);
        outputElement.appendChild(isSaveOutputValue);

        //Description
        QDomElement descriptionElement = doc.createElement("description");
        configNode.appendChild(descriptionElement);

        QDomCDATASection descriptionValue = doc.createCDATASection(tc_cfg.description);
        descriptionElement.appendChild(descriptionValue);

        //Extra parameters
        QDomElement extraParametersElement = doc.createElement("extra");
        configNode.appendChild(extraParametersElement);

        QDomCDATASection extraParametersValue = doc.createCDATASection(tc_cfg.extraParams);
        extraParametersElement.appendChild(extraParametersValue);

        configListNode.appendChild(configNode);
    }

    //Save
    QFile file(testCaseFullFileName);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void DBManager::CreateTestCaseInfrastructure(QString testCaseFullFileName, QMap<QString, QString> *testList)
{
    QString baseFolderName = GetStatusFullFileName(testCaseFullFileName, "", "");

    for(int i = 0; i < testList->count(); i++)
    {
        QString relativeTestName = testList->keys().at(i);

        //Create folder
        QDir folder = QFileInfo(QDir(baseFolderName).filePath(relativeTestName)).absoluteDir();
        folder.mkpath(".");

        //Create test status
        TestStatus ts;
        ts.data = testList->value(relativeTestName);
        ts.relativeFileName = relativeTestName;
        SaveTestStatus(testCaseFullFileName, &ts );
    }
}

TestCase *DBManager::GetTestCase(QString testCaseFullFileName)
{
    QDomDocument doc;

    QFileInfo tcFileInfo(testCaseFullFileName);
    if(tcFileInfo.exists())
    {
        QFile file(testCaseFullFileName);
        if(file.open(QIODevice::ReadOnly)) {
            if(doc.setContent(&file)) {
                TestCase *tc = new TestCase();
                tc->fullFileName = tcFileInfo.filePath();

                QDomElement tcRootElement = doc.firstChildElement("test-case");
                if(tcRootElement.isNull()) { return nullptr; }

                tc->ID = tcRootElement.firstChildElement("ID").text();

                QDomElement tcConfigListElement = tcRootElement.firstChildElement("config-list");
                tc->currConfigName = tcConfigListElement.attribute("current");

                QDomElement tcConfigElement = tcConfigListElement.firstChildElement("config");
                while(!tcConfigElement.isNull())
                {
                    TestCaseConfig cfg;

                    cfg.waitingTime = tcConfigElement.firstChildElement("waiting-sec").text().toInt();
                    cfg.maxThreads = tcConfigElement.firstChildElement("max-threads").text().toInt();
                    cfg.compressionLevel = tcConfigElement.firstChildElement("compression").text().toInt();
                    cfg.isSaveOutput = (tcConfigElement.firstChildElement("is-save-output").text().compare("true", Qt::CaseInsensitive) == 0)?true:false;
                    cfg.description = tcConfigElement.firstChildElement("description").firstChild().toCDATASection().data();
                    cfg.extraParams = tcConfigElement.firstChildElement("extra").firstChild().toCDATASection().data();

                    tc->configList.insert(tcConfigElement.attribute("name"), cfg);
                    tcConfigElement = tcConfigElement.nextSiblingElement("config");
                }

                return tc;
            }
            file.close();
        }
    }

    return nullptr;
}

void DBManager::SaveRunDescription(QString testCaseFullFileName, QString runName, RunDescription *rd)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootElement = doc.createElement("description");
    doc.appendChild(rootElement);

    //Start time UTC
    QDomElement startUtcElement = doc.createElement("start-utc");
    rootElement.appendChild(startUtcElement);

    QDomText startUtcValue = doc.createTextNode(rd->LocalDateTimeOfStart.toUTC().toString("yyyyMMdd_HHmmss"));
    startUtcElement.appendChild(startUtcValue);

    //Test plan full file name
    QDomElement planElement = doc.createElement("test-plan");
    rootElement.appendChild(planElement);

    QDomText planValue = doc.createTextNode(rd->TestPlanFullFileName);
    planElement.appendChild(planValue);

    //Comment
    QDomElement commentElement = doc.createElement("comment");
    rootElement.appendChild(commentElement);

    QDomCDATASection commentValue = doc.createCDATASection(rd->Comment);
    commentElement.appendChild(commentValue);

    //
    //Working time
    if(rd->WorkingTimeMs > 0)
    {
        QDomElement workingElement = doc.createElement("working-time-ms");
        rootElement.appendChild(workingElement);

        QDomText workingValue = doc.createTextNode(QString::number(rd->WorkingTimeMs));
        workingElement.appendChild(workingValue);
    }

    //Threads
    if((rd->MinUsedThreads > 0) && (rd->MaxUsedThreads >0))
    {
        QDomElement threadsElement = doc.createElement("threads");
        rootElement.appendChild(threadsElement);

        threadsElement.setAttribute("min", QString::number(rd->MinUsedThreads));
        threadsElement.setAttribute("max", QString::number(rd->MaxUsedThreads));
    }

    //Statistics
    QDomElement statisticsElement = doc.createElement("statistics");
    rootElement.appendChild(statisticsElement);

    for(int i = 0; i < rd->Statistics.count(); i++)
    {
        QString key = rd->Statistics.keys().at(i);
        int value = rd->Statistics[key];

        QDomElement itemElement = doc.createElement("item");
        statisticsElement.appendChild(itemElement);

        QDomText itemValue = doc.createTextNode(QString::number(value));
        itemElement.appendChild(itemValue);

        QString name = key.left(key.lastIndexOf(' '));
        QString color = key.right(key.length() - name.length() - 1);
        itemElement.setAttribute("name", name);
        itemElement.setAttribute("color", color);
    }

    //Save
    QString saveFullFileName = GetResultFullFileName(testCaseFullFileName, runName, "description", "xml");
    QFileInfo saveFileInfo(saveFullFileName);
    QDir().mkpath(saveFileInfo.absolutePath());

    QFile file(saveFullFileName);
    //if(file.exists()) { file.remove(); }
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

RunDescription *DBManager::GetRunDescription(QString testCaseFullFileName, QString runName)
{
    QString runDescFullFileName = GetResultFullFileName(testCaseFullFileName, runName, "description", "xml");

    QDomDocument doc;

    QFileInfo descFileInfo(runDescFullFileName);
    if(descFileInfo.exists())
    {
        QFile file(runDescFullFileName);
        if(file.open(QIODevice::ReadOnly)) {
            if(doc.setContent(&file)) {
                RunDescription *d = new RunDescription();

                d->Num = runName.toInt();

                QDomElement rootElement = doc.firstChildElement("description");
                if(rootElement.isNull()) { return nullptr; }

                QDomElement startElement = rootElement.firstChildElement("start-utc");
                QDateTime dt = QDateTime::fromString(startElement.text(), "yyyyMMdd_HHmmss");
                dt.setTimeSpec(Qt::UTC);
                d->LocalDateTimeOfStart = dt.toLocalTime();

                QDomElement testPlanElement = rootElement.firstChildElement("test-plan");
                d->TestPlanFullFileName = testPlanElement.text();

                QDomElement commentElement = rootElement.firstChildElement("comment");
                d->Comment = commentElement.firstChild().toCDATASection().data();

                QDomElement workingTimeElement = rootElement.firstChildElement("working-time-ms");
                d->WorkingTimeMs = workingTimeElement.text().toInt();

                QDomElement threadsElement = rootElement.firstChildElement("threads");
                d->MinUsedThreads = threadsElement.attribute("min").toInt();
                d->MaxUsedThreads = threadsElement.attribute("max").toInt();

                QDomElement statisticItemElement = rootElement.firstChildElement("statistics").firstChildElement("item");
                while(!statisticItemElement.isNull())
                {
                    QString key = statisticItemElement.attribute("name") + " " + statisticItemElement.attribute("color");
                    int value = statisticItemElement.text().toInt();
                    d->Statistics.insert(key, value);

                    statisticItemElement = statisticItemElement.nextSiblingElement("item");
                }
                return d;
            }
            file.close();
        }
    }
    return nullptr;
}

QStringList DBManager::GetTestCaseFolders(QString testCaseFullFileName, QString relativeOwnerFolderName)
{
    QString statusFullFolderName = GetStatusFullFileName(testCaseFullFileName, relativeOwnerFolderName, "");
    QDir dir(statusFullFolderName);

    return dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
}

QList<TestStatus *> DBManager::GetTestCaseFolderItems(QString testCaseFullFileName, QString relativeOwnerFolderName)
{
    QString statusFullFolderName = GetStatusFullFileName(testCaseFullFileName, relativeOwnerFolderName, "");
    QDir dir(statusFullFolderName);
    QList<TestStatus *> tcItems;

    QStringList nameFilter;
    nameFilter << "*.xml";
    QFileInfoList fileInfoList = dir.entryInfoList(nameFilter, QDir::Files);//, Qt::SortOrder::AscendingOrder);
    for(int i = 0; i < fileInfoList.count(); i++)
    {
        QString fileName = fileInfoList.at(i).fileName();
        TestStatus *testStatus = DBManager::GetTestStatus(
                    testCaseFullFileName, relativeOwnerFolderName + "/" + fileName.left(fileName.lastIndexOf(".")));
        if(testStatus != nullptr) { tcItems.append(testStatus); }
    }

    return tcItems;
}

TestStatus *DBManager::GetTestStatus(QString testCaseFullFileName, QString testRelativeName)
{
    QString testStatusFullFileName = GetStatusFullFileName(testCaseFullFileName, testRelativeName, "xml");

    QDomDocument doc;

    QFileInfo fileInfo(testStatusFullFileName);
    if(fileInfo.exists())
    {
        QFile file(testStatusFullFileName);
        if(file.open(QIODevice::ReadOnly)) {
            if(doc.setContent(&file)) {
                TestStatus *status = new TestStatus();

                status->relativeFileName = testRelativeName;

                QDomElement rootElement = doc.firstChildElement("status");
                if(rootElement.isNull()) { return nullptr; }

                status->data = rootElement.firstChildElement("data").firstChild().toCDATASection().data();

                QDomElement benchmarkElement = rootElement.firstChildElement("benchmarks").firstChildElement("benchmark");
                while(!benchmarkElement.isNull())
                {
                    BenchmarkInfo benchmarkInfo;
                    benchmarkInfo.status = benchmarkElement.attribute("status");
                    int runNum = benchmarkElement.attribute("run").toInt();
                    benchmarkInfo.outMark = benchmarkElement.attribute("out");

                    QDomElement labelElement = benchmarkElement.firstChildElement("label");
                    benchmarkInfo.label = labelElement.text();

                    QDomElement commentElement = benchmarkElement.firstChildElement("comment");
                    benchmarkInfo.comment = commentElement.text();

                    QDomElement colorElement = benchmarkElement.firstChildElement("color");
                    benchmarkInfo.color = QColor(colorElement.text());
                    status->benchmarks.insert(runNum, benchmarkInfo);

                    benchmarkElement = benchmarkElement.nextSiblingElement("benchmark");
                }

                QDomElement tagElement = rootElement.firstChildElement("tags").firstChildElement("tag");
                while(!tagElement.isNull())
                {
                    status->tags.append(tagElement.text());

                    tagElement = tagElement.nextSiblingElement("tag");
                }

                return status;
            }
            file.close();
        }
    }
    return nullptr;
}

void DBManager::SaveTestStatus(QString testCaseFullFileName, TestStatus *ts)
{
    QString statusFullFileName = GetStatusFullFileName(testCaseFullFileName, ts->relativeFileName, "xml");

    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("status");
    doc.appendChild(rootNode);

    //Data
    QDomElement dataElement = doc.createElement("data");
    rootNode.appendChild(dataElement);

    QDomCDATASection dataValue = doc.createCDATASection(ts->data);
    dataElement.appendChild(dataValue);

    //Benchmarks
    QDomElement benchmarksElement = doc.createElement("benchmarks");
    rootNode.appendChild(benchmarksElement);
    for(int i = 0; i < ts->benchmarks.count(); i++)
    {
        QDomElement benchmarkElement = doc.createElement("benchmark");
        benchmarksElement.appendChild(benchmarkElement);

        int key = ts->benchmarks.keys().at(i);

        benchmarkElement.setAttribute("run", GetRunName(key));
        benchmarkElement.setAttribute("out", ts->benchmarks.value(key).outMark);
        benchmarkElement.setAttribute("status", ts->benchmarks.value(key).status);

        //Status
        QDomElement labelElement = doc.createElement("label");
        benchmarkElement.appendChild(labelElement);

        QDomText statusValue = doc.createTextNode(ts->benchmarks.value(key).label);
        labelElement.appendChild(statusValue);

        //Comment
        QDomElement commentElement = doc.createElement("comment");
        benchmarkElement.appendChild(commentElement);

        QDomText commentValue = doc.createTextNode(ts->benchmarks.value(key).comment);
        commentElement.appendChild(commentValue);

        //Color
        QDomElement colorElement = doc.createElement("color");
        benchmarkElement.appendChild(colorElement);

        QDomText colorValue = doc.createTextNode(ts->benchmarks.value(key).color.name());
        colorElement.appendChild(colorValue);
    }

    //Tags
    QDomElement tagsElement = doc.createElement("tags");
    rootNode.appendChild(tagsElement);

    for(int i = 0; i < ts->tags.count(); i++)
    {
        QDomElement tagElement = doc.createElement("tag");
        tagsElement.appendChild(tagElement);

        QDomText tagValue = doc.createTextNode(ts->tags.at(i));
        tagElement.appendChild(tagValue);

    }

    QFile file(statusFullFileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

TestResult *DBManager::GetTestResult(QString testCaseFullFileName, QString relativeTestName, QString runName)
{
    QString testResultFullFileName = GetResultFullFileName(testCaseFullFileName, runName, relativeTestName, "xml");

    QDomDocument doc;

    QFileInfo fileInfo(testResultFullFileName);
    if(fileInfo.exists())
    {
        QFile file(testResultFullFileName);
        if(file.open(QIODevice::ReadOnly)) {
            if(doc.setContent(&file)) {
                TestResult *result = new TestResult();

                QDomElement rootElement = doc.firstChildElement("result");
                if(rootElement.isNull()) { return nullptr; }

                QDomElement outElement = rootElement.firstChildElement("out");
                result->outMark = outElement.text();

                QDomElement benchmarkElement = rootElement.firstChildElement("benchmark");
                if(!benchmarkElement.isNull()) {
                    result->benchmark = new TestCompareResult();
                    result->benchmark->status = benchmarkElement.attribute("status");
                    result->benchmark->outMark = benchmarkElement.attribute("out");
                    result->benchmark->runMark = benchmarkElement.attribute("run");
                    result->benchmark->compareResult = benchmarkElement.text().toInt();
                }

                QDomElement previousElement = rootElement.firstChildElement("previous");
                if(!previousElement.isNull()) {
                    result->previous = new TestCompareResult();
                    result->previous->status = "";
                    result->previous->outMark = previousElement.attribute("out");
                    result->previous->runMark = previousElement.attribute("run");
                    result->previous->compareResult = previousElement.text().toInt();
                }

                QDomElement exitElement = rootElement.firstChildElement("exit-code");
                result->exitCode = exitElement.text().toInt();

                QString exitStatusStr = exitElement.attribute("status");
                if(exitStatusStr == "not-started") {

                    result->exitStatus = ITestOutputComparator::ExitStatus::NOT_STARTED;

                } else if(exitStatusStr == "timeout-expired") {

                    result->exitStatus = ITestOutputComparator::ExitStatus::TIMEOUT_EXPIRED;

                } else if(exitStatusStr == "exit-code") {

                    result->exitStatus = ITestOutputComparator::ExitStatus::EXIT_CODE;

                } else { //if(exitStatusStr == "completed")

                    result->exitStatus = ITestOutputComparator::ExitStatus::COMPLETED;
                }

                QDomElement statusElement = rootElement.firstChildElement("test-status");
                result->status = statusElement.text();

                QDomElement commentElement = rootElement.firstChildElement("comment");
                result->comment = commentElement.text();

                QDomElement colorElement = rootElement.firstChildElement("test-color");
                result->color = QColor(colorElement.text());

                return result;
            }
            file.close();
        }
    }
    return nullptr;
}

QByteArray DBManager::GetConsoleLog(QString testCaseFullFileName, QString relativeTestName, QString runName)
{
    QString logFullFileName = GetResultFullFileName(testCaseFullFileName, runName, relativeTestName, "log");
    QByteArray logData;

    QFile file(logFullFileName);
    if(file.open(QIODevice::ReadOnly)) {
        logData = file.readAll();
        file.close();
    }

    return logData;
}

void DBManager::SaveTestingPlan(QString planFullFileName,QList<TestCase*> *testCases, QList<QPair<TestCase *, TestStatus *>> *tests, bool isRunOnce, bool isUpdateLastResult)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("run");
    rootNode.setAttribute("delete-after-run", isRunOnce?"true":"false");
    rootNode.setAttribute("update-last-result", isUpdateLastResult?"true":"false");
    rootNode.setAttribute("count", tests->count());
    doc.appendChild(rootNode);

    for(int i = 0; i < testCases->count(); i++)
    {
        TestCase *tc = testCases->at(i);

        //Test-Case
        QDomElement testCaseNode = doc.createElement("test-case");
        rootNode.appendChild(testCaseNode);

        //Type (ID)
        QDomElement typeElement = doc.createElement("ID");
        testCaseNode.appendChild(typeElement);

        QDomText typeValue = doc.createTextNode(tc->ID);
        typeElement.appendChild(typeValue);

        //Test Case File Name
        QDomElement fileNameElement = doc.createElement("path");
        testCaseNode.appendChild(fileNameElement);

        QDomText fileNameValue = doc.createTextNode(tc->fullFileName);
        fileNameElement.appendChild(fileNameValue);

        TestCaseConfig currentTestCaseConfig = tc->configList.value(tc->currConfigName);

        //Wating time
        QDomElement waitingElement = doc.createElement("waiting-sec");
        testCaseNode.appendChild(waitingElement);

        QDomText waitingValue = doc.createTextNode(QString::number(currentTestCaseConfig.waitingTime));
        waitingElement.appendChild(waitingValue);

        //Threads
        QDomElement threadsElement = doc.createElement("max-threads");
        testCaseNode.appendChild(threadsElement);

        QDomText threadsValue = doc.createTextNode(QString::number(currentTestCaseConfig.maxThreads));
        threadsElement.appendChild(threadsValue);

        //Compression
        QDomElement compressionElement = doc.createElement("compression");
        testCaseNode.appendChild(compressionElement);

        QDomText compressionValue = doc.createTextNode(QString::number(currentTestCaseConfig.compressionLevel));
        compressionElement.appendChild(compressionValue);

        //Is save output
        QDomElement isSaveOutputElement = doc.createElement("is-save-output");
        testCaseNode.appendChild(isSaveOutputElement);

        QDomText isSaveOutputValue = doc.createTextNode(currentTestCaseConfig.isSaveOutput?"true":"false");
        isSaveOutputElement.appendChild(isSaveOutputValue);

        //Extra params
        QDomElement extraParamElement = doc.createElement("extra");
        testCaseNode.appendChild(extraParamElement);

        QDomCDATASection extraParamValue = doc.createCDATASection(currentTestCaseConfig.extraParams);
        extraParamElement.appendChild(extraParamValue);

        //Test list
        QDomElement testListNode = doc.createElement("test-list");
        testCaseNode.appendChild(testListNode);

        int tcTestsCount = 0;
        for(int j = 0; j < tests->count(); j++)
        {
            if(tests->at(j).first == tc) {
                TestStatus *status = tests->at(j).second;
                QDomElement testElement = doc.createElement("test");
                testElement.setAttribute("name", status->relativeFileName);
                testListNode.appendChild(testElement);

                QDomCDATASection testDataValue = doc.createCDATASection(status->data);
                testElement.appendChild(testDataValue);

                tcTestsCount++;
            }
        }
        testCaseNode.setAttribute("count", tcTestsCount);
    }

    //Save
    QFile file(planFullFileName);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

QDomDocument *DBManager::GetTestingPlan(QString planFullFileName)
{
    QDomDocument *doc = new QDomDocument;

    QFileInfo fileInfo(planFullFileName);
    if(fileInfo.exists())
    {
        QFile file(planFullFileName);
        if(file.open(QIODevice::ReadOnly))
        {
            doc->setContent(&file);
            file.close();
        }
    }

    return doc;
}

void DBManager::SaveTestConsoleOutput(QString testCaseFullFileName, QString runName, QString relativeTestName, QStringList *consoleOutput)
{
    QString saveFullFileName = GetResultFullFileName(testCaseFullFileName, runName, relativeTestName, "log");
    QFileInfo saveFileInfo(saveFullFileName);
    QDir().mkpath(saveFileInfo.absolutePath());

    QFile file(saveFullFileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        for(int i = 0; i < consoleOutput->count(); i++) { out << consoleOutput->at(i) << "\n"; }
        file.close();
    }
}

void DBManager::SaveTestResult(QString testCaseFullFileName, QString runName, QString relativeTestName, TestResult *tr)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("result");
    doc.appendChild(rootNode);

    //Output link
    QDomElement outputElement = doc.createElement("out");
    rootNode.appendChild(outputElement);

    QDomText outputValue = doc.createTextNode(tr->outMark);
    outputElement.appendChild(outputValue);

    //Benchmark
    if(tr->benchmark != nullptr) {

        QDomElement benchmarkElement = doc.createElement("benchmark");
        rootNode.appendChild(benchmarkElement);

        QDomText benchmarkValue = doc.createTextNode(QString::number(tr->benchmark->compareResult));
        benchmarkElement.appendChild(benchmarkValue);

        benchmarkElement.setAttribute("run", tr->benchmark->runMark);
        benchmarkElement.setAttribute("out", tr->benchmark->outMark);
        benchmarkElement.setAttribute("status", tr->benchmark->status);
    }

    //Previous
    if(tr->previous != nullptr) {

        QDomElement previousElement = doc.createElement("previous");
        rootNode.appendChild(previousElement);

        QDomText previousValue = doc.createTextNode(QString::number(tr->previous->compareResult));
        previousElement.appendChild(previousValue);

        previousElement.setAttribute("run", tr->previous->runMark);
        previousElement.setAttribute("out", tr->previous->outMark);
    }

    //Exit code
    QDomElement exitCodeElement = doc.createElement("exit-code");
    rootNode.appendChild(exitCodeElement);

    QDomText exitCodeValue = doc.createTextNode(QString::number(tr->exitCode));
    exitCodeElement.appendChild(exitCodeValue);
    switch(tr->exitStatus) {
        case ITestOutputComparator::ExitStatus::NOT_STARTED:
            exitCodeElement.setAttribute("status", "not-started");
            break;

        case ITestOutputComparator::ExitStatus::TIMEOUT_EXPIRED:
            exitCodeElement.setAttribute("status", "timeout-expired");
            break;

        case ITestOutputComparator::ExitStatus::EXIT_CODE:
            exitCodeElement.setAttribute("status", "exit-code");
            break;

        default: //ITestOutputComparator::ExitStatus::COMPLETED
            exitCodeElement.setAttribute("status", "completed");
            break;
    }

    //Exec time
    QDomElement exeTimeElement = doc.createElement("exec-time-ms");
    rootNode.appendChild(exeTimeElement);

    QDomText exeTimeValue = doc.createTextNode(QString::number(tr->execTimeMs));
    exeTimeElement.appendChild(exeTimeValue);

    //Status
    QDomElement statusElement = doc.createElement("test-status");
    rootNode.appendChild(statusElement);

    QDomText statusValue = doc.createTextNode(tr->status);
    statusElement.appendChild(statusValue);

    //Comment
    QDomElement commentElement = doc.createElement("comment");
    rootNode.appendChild(commentElement);

    QDomText commentValue = doc.createTextNode(tr->comment);
    commentElement.appendChild(commentValue);

    //Color
    QDomElement colorElement = doc.createElement("test-color");
    rootNode.appendChild(colorElement);

    QDomText colorValue = doc.createTextNode(tr->color.name());
    colorElement.appendChild(colorValue);

    //Save
    QString saveFullFileName = GetResultFullFileName(testCaseFullFileName, runName, relativeTestName, "xml");
    QFileInfo saveFileInfo(saveFullFileName);
    QDir().mkpath(saveFileInfo.absolutePath());

    QFile file(saveFullFileName);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

QVector<int> *DBManager::GetRunList(QString testCaseFullFileName)
{
    QVector<int> *result = new QVector<int>();

    QDir resultDir(GetResultFullFileName(testCaseFullFileName, "", "", ""));
    QStringList runNameList = resultDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < runNameList.count(); i++) { result->append(runNameList.at(i).toInt()); }

    return result;
}

QVector<int> *DBManager::GetOutList(QString testCaseFullFileName, QString relativeTestName)
{
    QVector<int> *result = new QVector<int>();

    QDir outputDir(GetOutFolder(testCaseFullFileName, relativeTestName, ""));
    QStringList outputNameList = outputDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < outputNameList.count(); i++) { result->append(outputNameList.at(i).toInt()); }

    return result;
}

TagCollection *DBManager::LoadTagCollection(QString fullFileName)
{
    TagCollection *tagCollection = nullptr;

    QFile file(fullFileName);
    if(file.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if(doc.setContent(&file)) {
            tagCollection = new TagCollection();
            tagCollection->owner = tagCollection;

            tagCollection->name = QFileInfo(fullFileName).baseName();
            tagCollection->fullFileName = fullFileName;

            //Parse Xml Document
            QDomElement collectionElement = doc.firstChildElement("collection");
            if(collectionElement.isNull()) { return tagCollection; }

            QDomElement descriptionElement = collectionElement.firstChildElement("description");
            if(!descriptionElement.isNull())
            {
                tagCollection->description = descriptionElement.firstChild().toCDATASection().data();
            }

            QDomElement tagsElement = collectionElement.firstChildElement("tags");
            if(!tagsElement.isNull()) { LoadTagFolderElements(tagsElement, tagCollection, tagCollection); }
        }
        file.close();
    }
    return tagCollection;
}

void DBManager::LoadTagFolderElements(QDomElement &folderElement, TagFolder *tagFolder, TagCollection *owner)
{
    QDomElement tagListElement = folderElement.firstChildElement();
    while(!tagListElement.isNull())
    {
        QString nodeName = tagListElement.nodeName();
        if(nodeName.compare("tag", Qt::CaseInsensitive) == 0)
        {
            Tag *tag = new Tag();
            tag->owner = owner;
            tag->parent = tagFolder;
            tag->type = tagListElement.attribute("type", "");
            tag->name = tagListElement.attribute("name","-");
            tag->data = tagListElement.firstChild().toCDATASection().data();

            tagFolder->tags.append(tag);
        }
        else if(nodeName.compare("folder", Qt::CaseInsensitive) == 0)
        {
            TagFolder *subFolder = new TagFolder();
            subFolder->owner = owner;
            subFolder->parent = tagFolder;
            subFolder->name = tagListElement.attribute("name","-");

            QDomElement folderDescriptionElement = tagListElement.firstChildElement("description");
            if(!folderDescriptionElement.isNull())
            {
                subFolder->description = folderDescriptionElement.firstChild().toCDATASection().data();
            }

            LoadTagFolderElements(tagListElement, subFolder, owner);

            tagFolder->folders.append(subFolder);
        }
        tagListElement = tagListElement.nextSiblingElement();
    }
}

void DBManager::SaveTagCollection(TagCollection *tagCollection)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootElement = doc.createElement("collection");
    doc.appendChild(rootElement);

    //Description
    QDomElement descriptionNode = doc.createElement("description");
    rootElement.appendChild(descriptionNode);

    QDomCDATASection descriptionValue = doc.createCDATASection(tagCollection->description);
    descriptionNode.appendChild(descriptionValue);

    QDomElement tagsNode = doc.createElement("tags");
    rootElement.appendChild(tagsNode);

    SaveTagFolderElements(tagsNode, tagCollection);

    //Save
    QFileInfo saveFileInfo(tagCollection->fullFileName);
    QDir().mkpath(saveFileInfo.absolutePath());

    QFile file(tagCollection->fullFileName);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void DBManager::SaveTagFolderElements(QDomElement &folderElement, TagFolder *tagFolder)
{
    for(int i = 0; i < tagFolder->folders.length(); i++)
    {
        TagFolder *subFolder = tagFolder->folders.at(i);

        QDomElement subFolderElement = folderElement.ownerDocument().createElement("folder");
        folderElement.appendChild(subFolderElement);

        subFolderElement.setAttribute("name", subFolder->name);

        //Description
        QDomElement descriptionNode = folderElement.ownerDocument().createElement("description");
        subFolderElement.appendChild(descriptionNode);

        QDomCDATASection descriptionValue = folderElement.ownerDocument().createCDATASection(subFolder->description);
        descriptionNode.appendChild(descriptionValue);

        //Sub folders
        SaveTagFolderElements(subFolderElement, subFolder);
    }

    for(int i = 0; i < tagFolder->tags.length(); i++)
    {
        Tag *tag = tagFolder->tags.at(i);

        QDomElement tagElement = folderElement.ownerDocument().createElement("tag");
        folderElement.appendChild(tagElement);

        tagElement.setAttribute("type", tag->type);
        tagElement.setAttribute("name", tag->name);

        QDomCDATASection dataValue = folderElement.ownerDocument().createCDATASection(tag->data);
        tagElement.appendChild(dataValue);
    }
}

void DBManager::DeleteTagCollection(QString fullFileName)
{
    QFile(fullFileName).remove();
}
