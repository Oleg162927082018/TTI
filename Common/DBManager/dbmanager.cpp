#include "dbmanager.h"

#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

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
    configListNode.setAttribute("current", tc->CurrentConfigName);
    rootNode.appendChild(configListNode);

    for(int i = 0; i < tc->ConfigList.count(); i++)
    {
        QString name = tc->ConfigList.keys().at(i);
        TestCaseConfig tc_cfg = tc->ConfigList.value(name);

        QDomElement configNode = doc.createElement("config");
        configNode.setAttribute("name", name);

        //Exit Wating Time
        QDomElement watingElement = doc.createElement("waiting-sec");
        configNode.appendChild(watingElement);

        QDomText watingValue = doc.createTextNode(QString::number(tc_cfg.WaitingTime));
        watingElement.appendChild(watingValue);

        //Max Threads
        QDomElement threadsElement = doc.createElement("max-threads");
        configNode.appendChild(threadsElement);

        QDomText threadsValue = doc.createTextNode(QString::number(tc_cfg.MaxThreads));
        threadsElement.appendChild(threadsValue);

        //Compression Level
        QDomElement compressionElement = doc.createElement("compression");
        configNode.appendChild(compressionElement);

        QDomText compressionValue = doc.createTextNode(QString::number(tc_cfg.CompressionLevel));
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

        //Extra params
        QDomElement extraNode = doc.createElement("extra");
        extraNode.appendChild(tc_cfg.ExtraParams);
        configNode.appendChild(extraNode);

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

void DBManager::SaveTestCaseCache(QString testCaseFullFileName, QMap<QString, QDomDocument *> *testList)
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
        ts.data = *testList->value(relativeTestName);
        SaveTestStatus(testCaseFullFileName, relativeTestName, &ts );
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

                QDomElement tcRootElement = doc.firstChildElement("test-case");
                if(tcRootElement.isNull()) { return NULL; }

                tc->ID = tcRootElement.firstChildElement("ID").text();

                QDomElement tcConfigListElement = tcRootElement.firstChildElement("config-list");
                tc->CurrentConfigName = tcConfigListElement.attribute("current");

                QDomElement tcConfigElement = tcConfigListElement.firstChildElement("config");
                while(!tcConfigElement.isNull())
                {
                    TestCaseConfig cfg;

                    cfg.WaitingTime = tcConfigElement.firstChildElement("waiting-sec").text().toInt();
                    cfg.MaxThreads = tcConfigElement.firstChildElement("max-threads").text().toInt();
                    cfg.CompressionLevel = tcConfigElement.firstChildElement("compression").text().toInt();
                    cfg.isSaveOutput = (tcConfigElement.firstChildElement("is-save-output").text().compare("true", Qt::CaseInsensitive) == 0)?true:false;
                    cfg.description = tcConfigElement.firstChildElement("description").firstChild().toCDATASection().data();

                    QDomNode extraParamNode = tcConfigElement.firstChildElement("extra").firstChild();
                    while(!extraParamNode.isNull())
                    {
                        QDomNode newNode = cfg.ExtraParams.importNode(extraParamNode, true);
                        cfg.ExtraParams.appendChild(newNode);
                        extraParamNode = extraParamNode.nextSibling();
                    }

                    tc->ConfigList.insert(tcConfigElement.attribute("name"), cfg);
                    tcConfigElement = tcConfigElement.nextSiblingElement("config");
                }

                return tc;
            }
            file.close();
        }
    }

    return NULL;
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
                if(rootElement.isNull()) { return NULL; }

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
    return NULL;
}

TestCaseFolder *DBManager::GetTestCaseCache(QString testCaseFullFileName)
{
    QDir statusFolder(GetStatusFullFileName(testCaseFullFileName, "", ""));

    TestCaseFolder *f = GetCacheFolder(statusFolder.absolutePath(), "");
    return f;
}

TestCaseFolder *DBManager::GetCacheFolder(QString statusFullFolderName, QString relativeSubFolderName)
{
    QDir dir(statusFullFolderName + relativeSubFolderName);

    TestCaseFolder *resultFolder = new TestCaseFolder();
    resultFolder->Name = dir.dirName();
    resultFolder->ParentFolder = NULL;

    QStringList folderNames = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    for(int i = 0; i < folderNames.count(); i++)
    {
        TestCaseFolder *subFolder = GetCacheFolder(statusFullFolderName, relativeSubFolderName + "/" + folderNames.at(i));
        subFolder->ParentFolder = resultFolder;

        resultFolder->SubFolderList.append(subFolder);
        for(int j = 0; j < subFolder->TestList.count(); j++)
        { resultFolder->TestList.append(subFolder->TestList.at(j)); }
    }

    QStringList nameFilter;
    nameFilter << "*.xml";
    QFileInfoList fileInfoList = dir.entryInfoList(nameFilter, QDir::Files);//, Qt::SortOrder::AscendingOrder);
    for(int i = 0; i < fileInfoList.count(); i++)
    {
        QString fileName = fileInfoList.at(i).fileName();
        TestCaseItem *test = new TestCaseItem();
        test->RelativeFileName = relativeSubFolderName + "/" + fileName.left(fileName.lastIndexOf("."));
        test->Name = QFileInfo(test->RelativeFileName).fileName();
        resultFolder->TestList.append(test);
    }

    return resultFolder;
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
                if(rootElement.isNull()) { return NULL; }

                QDomElement outElement = rootElement.firstChildElement("out");
                result->outMark = outElement.text();

                QDomElement benchmarkElement = rootElement.firstChildElement("benchmark");
                result->benchmarkOutMark = benchmarkElement.attribute("out");
                result->benchmarkRunMark = benchmarkElement.attribute("run");
                result->benchmarkStatus = benchmarkElement.attribute("status");
                result->benchmarkCompareResult = benchmarkElement.text().toInt();


                QDomElement previousElement = rootElement.firstChildElement("previous");
                result->previousOutMark = previousElement.attribute("out");
                result->previousRunMark = previousElement.attribute("run");
                result->previousCompareResult = previousElement.text().toInt();

                QDomElement exitElement = rootElement.firstChildElement("exit-code");
                result->exitStatus = exitElement.attribute("status");
                result->exitCode = exitElement.text().toInt();

                QDomElement statusElement = rootElement.firstChildElement("test-status");
                result->status = statusElement.text();

                QDomElement colorElement = rootElement.firstChildElement("test-color");
                result->color = QColor(colorElement.text());

                return result;
            }
            file.close();
        }
    }
    return NULL;
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

                QDomElement rootElement = doc.firstChildElement("status");
                if(rootElement.isNull()) { return NULL; }

                QDomNode dataNode = rootElement.firstChildElement("data").firstChild();
                while(!dataNode.isNull())
                {
                    QDomNode node = status->data.importNode(dataNode, true);
                    status->data.appendChild(node);
                    dataNode = dataNode.nextSibling();
                }

                QDomElement benchmarkElement = rootElement.firstChildElement("benchmarks").firstChildElement("benchmark");
                while(!benchmarkElement.isNull())
                {
                    BenchmarkInfo benchmarkInfo;
                    benchmarkInfo.status = benchmarkElement.attribute("status");
                    int runNum = benchmarkElement.attribute("run").toInt();
                    benchmarkInfo.outMark = benchmarkElement.attribute("out");
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
    return NULL;
}

void DBManager::SaveTestStatus(QString testCaseFullFileName, QString testRelativeName, TestStatus *ts)
{
    QString statusFullFileName = GetStatusFullFileName(testCaseFullFileName, testRelativeName, "xml");

    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("status");
    doc.appendChild(rootNode);

    //Data
    QDomElement dataElement = doc.createElement("data");
    dataElement.appendChild(ts->data);
    rootNode.appendChild(dataElement);


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

void DBManager::SaveTestingPlan(QString planFullFileName, QList<TestCaseFolder *> *cache, bool isDeleteAfterRun, bool isUpdateLastResult)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("run");
    rootNode.setAttribute("delete-after-run", isDeleteAfterRun?"true":"false");
    rootNode.setAttribute("update-last-result", isUpdateLastResult?"true":"false");
    doc.appendChild(rootNode);

    int planCount = 0;
    for(int i = 0; i < cache->count(); i++)
    {
        if(cache->at(i)->Checked)
        {
            TestCase *tc = cache->at(i)->ownerTestCase;

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

            QDomText fileNameValue = doc.createTextNode(tc->FullFileName);
            fileNameElement.appendChild(fileNameValue);

            TestCaseConfig currentTestCaseConfig = tc->ConfigList.value(tc->CurrentConfigName);

            //Wating time
            QDomElement waitingElement = doc.createElement("waiting-sec");
            testCaseNode.appendChild(waitingElement);

            QDomText waitingValue = doc.createTextNode(QString::number(currentTestCaseConfig.WaitingTime));
            waitingElement.appendChild(waitingValue);

            //Threads
            QDomElement threadsElement = doc.createElement("max-threads");
            testCaseNode.appendChild(threadsElement);

            QDomText threadsValue = doc.createTextNode(QString::number(currentTestCaseConfig.MaxThreads));
            threadsElement.appendChild(threadsValue);

            //Compression
            QDomElement compressionElement = doc.createElement("compression");
            testCaseNode.appendChild(compressionElement);

            QDomText compressionValue = doc.createTextNode(QString::number(currentTestCaseConfig.CompressionLevel));
            compressionElement.appendChild(compressionValue);

            //Is save output
            QDomElement isSaveOutputElement = doc.createElement("is-save-output");
            testCaseNode.appendChild(isSaveOutputElement);

            QDomText isSaveOutputValue = doc.createTextNode(currentTestCaseConfig.isSaveOutput?"true":"false");
            isSaveOutputElement.appendChild(isSaveOutputValue);

            //Extra params
            QDomElement extraParamElement = doc.createElement("extra");
            testCaseNode.appendChild(extraParamElement);

            extraParamElement.appendChild(currentTestCaseConfig.ExtraParams);

            //Test list
            QDomElement testListNode = doc.createElement("test-list");
            testCaseNode.appendChild(testListNode);

            TestCaseFolder *tcFolder = cache->at(i);
            int testCaseCount = tcFolder->TestList.count();
            testCaseNode.setAttribute("count", testCaseCount);
            planCount += testCaseCount;
            for(int j = 0; j < testCaseCount; j++)
            {
                TestCaseItem *item = tcFolder->TestList.at(j);
                if(item->Checked)
                {
                    QDomElement testElement = doc.createElement("test");
                    testElement.setAttribute("name", item->RelativeFileName);
                    testElement.setAttribute("status", "-");
                    testListNode.appendChild(testElement);

                    if(item->status != NULL)
                    {
                        testElement.appendChild(item->status->data);
                    }

                }
            }
        }
    }

    rootNode.setAttribute("count", planCount);

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
    if(!tr->benchmarkRunMark.isEmpty())
    {
        QDomElement benchmarkElement = doc.createElement("benchmark");
        rootNode.appendChild(benchmarkElement);

        QDomText benchmarkValue = doc.createTextNode(QString::number(tr->benchmarkCompareResult));
        benchmarkElement.appendChild(benchmarkValue);

        benchmarkElement.setAttribute("run", tr->benchmarkRunMark);
        benchmarkElement.setAttribute("out", tr->benchmarkOutMark);
        benchmarkElement.setAttribute("status", tr->benchmarkStatus);
    }

    //Previous
    if(!tr->previousRunMark.isEmpty())
    {
        QDomElement previousElement = doc.createElement("previous");
        rootNode.appendChild(previousElement);

        QDomText previousValue = doc.createTextNode(QString::number(tr->previousCompareResult));
        previousElement.appendChild(previousValue);

        previousElement.setAttribute("run", tr->previousRunMark);
        previousElement.setAttribute("out", tr->previousOutMark);
    }

    //Exit code
    QDomElement exitCodeElement = doc.createElement("exit-code");
    rootNode.appendChild(exitCodeElement);

    QDomText exitCodeValue = doc.createTextNode(QString::number(tr->exitCode));
    exitCodeElement.appendChild(exitCodeValue);

    exitCodeElement.setAttribute("status", tr->exitStatus);

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

void DBManager::SaveTagCollection(TagItem *tagCollection)
{
    QDomDocument doc;

    //Root
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.insertBefore(xmlNode, doc.firstChild());

    QDomElement rootNode = doc.createElement("collection");
    doc.appendChild(rootNode);

    //Description
    QDomElement descriptionNode = doc.createElement("description");
    rootNode.appendChild(descriptionNode);

    QDomCDATASection descriptionValue = doc.createCDATASection(tagCollection->collection->description);
    descriptionNode.appendChild(descriptionValue);

    QDomElement tagsNode = doc.createElement("tags");
    rootNode.appendChild(tagsNode);

    SaveTagFolderElements(tagsNode, tagCollection);

    //Save
    QFileInfo saveFileInfo(tagCollection->collection->fullFileName);
    QDir().mkpath(saveFileInfo.absolutePath());

    QFile file(tagCollection->collection->fullFileName);
    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << doc.toString();
        file.close();
    }
}

void DBManager::SaveTagFolderElements(QDomElement &folderElement, TagItem *tagItem)
{
    for(int i = 0; i < tagItem->subItems.length(); i++)
    {
        TagItem *subItem = tagItem->subItems.at(i);

        if(subItem->tag != NULL)
        {
            QDomElement tagElement = folderElement.ownerDocument().createElement("tag");
            folderElement.appendChild(tagElement);

            tagElement.setAttribute("type", subItem->tag->type);
            tagElement.setAttribute("name", subItem->name);

            tagElement.appendChild(subItem->tag->data);
        }
        else //folder
        {
            QDomElement subFolderElement = folderElement.ownerDocument().createElement("folder");
            folderElement.appendChild(subFolderElement);

            subFolderElement.setAttribute("name", subItem->name);

            //Description
            QDomElement descriptionNode = folderElement.ownerDocument().createElement("description");
            subFolderElement.appendChild(descriptionNode);

            QDomCDATASection descriptionValue = folderElement.ownerDocument().createCDATASection(subItem->folder->description);
            descriptionNode.appendChild(descriptionValue);

            //Sub folders
            SaveTagFolderElements(subFolderElement, subItem);
        }
    }
}

TagItem *DBManager::GetTagCollection(QString fullFileName)
{
    TagItem *tagCollection = NULL;

    QFile file(fullFileName);
    if(file.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        if(doc.setContent(&file)) {
            tagCollection = new TagItem();
            tagCollection->collection = new TagCollection();

            tagCollection->name = QFileInfo(fullFileName).baseName();
            tagCollection->path = tagCollection->name;
            tagCollection->collection->fullFileName = fullFileName;

            //Parse Xml Document
            QDomElement collectionElement = doc.firstChildElement("collection");
            if(collectionElement.isNull()) { return tagCollection; }

            QDomElement descriptionElement = collectionElement.firstChildElement("description");
            if(!descriptionElement.isNull())
            {
                tagCollection->collection->description = descriptionElement.firstChild().toCDATASection().data();
            }

            QDomElement tagsElement = collectionElement.firstChildElement("tags");
            if(!tagsElement.isNull()) { LoadTagFolderElements(tagsElement, tagCollection); }
        }
        file.close();
    }
    return tagCollection;
}

void DBManager::LoadTagFolderElements(QDomElement &folderElement, TagItem *tagFolder)
{
    QDomElement tagListElement = folderElement.firstChildElement();
    while(!tagListElement.isNull())
    {
        QString nodeName = tagListElement.nodeName();
        if(nodeName.compare("tag", Qt::CaseInsensitive) == 0)
        {
            TagItem *tagItem = new TagItem();
            tagItem->parent = tagFolder;
            tagItem->collection = tagFolder->collection;

            tagItem->tag = new Tag();
            tagItem->tag->type = tagListElement.attribute("type", "");
            tagItem->name = tagListElement.attribute("name","-");
            tagItem->path = tagFolder->path + ":" + tagItem->name;
            for(int i = 0; i < tagListElement.childNodes().length(); i++)
            {
                tagItem->tag->data.appendChild(tagItem->tag->data.importNode(tagListElement.childNodes().at(i), true));
            }

            tagFolder->subItems.append(tagItem);
        }
        else if(nodeName.compare("folder", Qt::CaseInsensitive) == 0)
        {
            TagItem *subFolder = new TagItem();
            subFolder->parent = tagFolder;
            subFolder->collection = tagFolder->collection;

            subFolder->folder = new TagFolder();
            subFolder->name = tagListElement.attribute("name","-");
            subFolder->path = tagFolder->path + ":" + subFolder->name;

            QDomElement folderDescriptionElement = tagListElement.firstChildElement("description");
            if(!folderDescriptionElement.isNull())
            {
                subFolder->folder->description = folderDescriptionElement.firstChild().toCDATASection().data();
            }

            LoadTagFolderElements(tagListElement, subFolder);

            tagFolder->subItems.append(subFolder);
        }
        tagListElement = tagListElement.nextSiblingElement();
    }

}

void DBManager::DeleteTagCollection(QString fullFileName)
{
    QFile(fullFileName).remove();
}
