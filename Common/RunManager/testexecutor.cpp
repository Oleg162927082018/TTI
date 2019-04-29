#include "runmanager.h"
#include "testexecutor.h"

#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

#include <windows.h>

TestExecutor::TestExecutor(TestInfo *info) : QObject(nullptr)
{
    this->info = info;
}

void TestExecutor::doKill()
{
    if(proc != nullptr) { proc->kill(); }
}

void TestExecutor::run()
{
    /*SHELLEXECUTEINFO shExecInfo;
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

    shExecInfo.fMask = nullptr;
    shExecInfo.hwnd = nullptr;
    shExecInfo.lpVerb = L"runas";
    shExecInfo.lpFile = L"D:/Home/Total_Test_Integrator/lib/PhantomJS/test-ready/phantomjs.exe";
    shExecInfo.lpParameters = L"\"D:/Home/Total_Test_Integrator/lib/PhantomJS/test-ready/scripts/OpenStartPage/OpenStartPage.js\" http://ya.ru \"D:/temp\"";
    shExecInfo.lpDirectory = nullptr;
    shExecInfo.nShow = SW_HIDE;
    shExecInfo.hInstApp = nullptr;

    ShellExecuteEx(&shExecInfo);*/

    QObject obj;
    proc = new QProcess(&obj);

    /*proc->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments *args)
    {
        args->flags |= CREATE_NEW_CONSOLE;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;

        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED
                                             | FOREGROUND_INTENSITY;
    });*/

    info->testResult.execTimeMs = QDateTime::currentMSecsSinceEpoch();
    proc->setWorkingDirectory(info->workingFullFolderName);
    proc->start(info->exeCommand, info->argumentList);

    if (!proc->waitForStarted(5000))
    {
        info->testResult.execTimeMs = 0;
        info->testResult.exitStatus = ITestOutputComparator::ExitStatus::NOT_STARTED;
        info->testResult.exitCode = -1;

        info->consoleOutput.append("Test not started:");
        info->consoleOutput.append(info->exeCommand);
        info->consoleOutput.append(info->argumentList);
        info->consoleOutput.append(proc->errorString());

        //Not compare with benchmark
        if(info->testResult.benchmark != nullptr) {
            delete info->testResult.benchmark;
            info->testResult.benchmark = nullptr;
        }

        //Not compare with previous
        if(info->testResult.previous != nullptr) {
            delete info->testResult.previous;
            info->testResult.previous = nullptr;
        }

        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                    info->consoleOutput, info->outputFullFolderName, nullptr, nullptr, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                    outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        goto theEnd;
    }

    proc->closeWriteChannel();

    if (!proc->waitForFinished(info->waitingTime * 1000))
    {
        info->testResult.execTimeMs = info->waitingTime * 1000;
        info->testResult.exitStatus = ITestOutputComparator::ExitStatus::TIMEOUT_EXPIRED;
        info->testResult.exitCode = -1;

        info->consoleOutput.append("Timeout expired:");
        info->consoleOutput.append(info->exeCommand);
        info->consoleOutput.append(info->argumentList);

        //Not compare with benchmark
        if(info->testResult.benchmark != nullptr) {
            delete info->testResult.benchmark;
            info->testResult.benchmark = nullptr;
        }

        //Not compare with previous
        if(info->testResult.previous != nullptr) {
            delete info->testResult.previous;
            info->testResult.previous = nullptr;
        }

        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                    info->consoleOutput, info->outputFullFolderName, nullptr, nullptr, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                    outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        goto theEnd;
    }

    info->testResult.execTimeMs = QDateTime::currentMSecsSinceEpoch() - info->testResult.execTimeMs;
    info->consoleOutput.append(QString::fromLocal8Bit(proc->readAll().data()).split('\n'));
    info->testResult.exitCode = proc->exitCode();

    if(info->testResult.exitCode != 0) {

        info->testResult.exitStatus = ITestOutputComparator::ExitStatus::EXIT_CODE;

        //Not compare with benchmark
        if(info->testResult.benchmark != nullptr) {
            delete info->testResult.benchmark;
            info->testResult.benchmark = nullptr;
        }

        //Not compare with previous
        if(info->testResult.previous != nullptr) {
            delete info->testResult.previous;
            info->testResult.previous = nullptr;
        }

        //if exit code != 0 or compression level == 0 need not compare with any thing
        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                   info->consoleOutput, info->outputFullFolderName, nullptr, nullptr, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                   outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        info->testResult.outMark = QDir(info->outputFullFolderName).dirName();

        goto theEnd;
    }

    info->testResult.exitStatus = ITestOutputComparator::ExitStatus::COMPLETED;

    if(info->compressionLevel == 0) {

        //Not compare with benchmark
        if(info->testResult.benchmark != nullptr) {
            delete info->testResult.benchmark;
            info->testResult.benchmark = nullptr;
        }

        //Not compare with previous
        if(info->testResult.previous != nullptr) {
            delete info->testResult.previous;
            info->testResult.previous = nullptr;
        }

        //if exit code != 0 or compression level == 0 need not compare with any thing
        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                   info->consoleOutput, info->outputFullFolderName, nullptr, nullptr, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                   outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        info->testResult.outMark = QDir(info->outputFullFolderName).dirName();

    } else if(info->compressionLevel == 3) {

        //Compare with all and delete if equal output found
        if((info->testResult.benchmark != nullptr) && !info->benchmarkOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull())
        {
            info->testResult.benchmark->compareResult =
                    info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName);
        }

        if((info->testResult.previous != nullptr) && !info->previousOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull())
        {
            info->testResult.previous->compareResult =
                    info->comparator->Compare(info->previousOutputFullFolderName, info->outputFullFolderName);
        }

        int *benchmarkCompare = nullptr;
        if(info->testResult.benchmark != nullptr) {
            benchmarkCompare = new int;
            *benchmarkCompare = info->testResult.benchmark->compareResult;
        }

        int *prevCompare = nullptr;
        if(info->testResult.previous != nullptr) {
            prevCompare = new int;
            *prevCompare = info->testResult.previous->compareResult;
        }

        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                   info->consoleOutput, info->outputFullFolderName, benchmarkCompare, prevCompare, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                   outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        if(benchmarkCompare != nullptr) { delete benchmarkCompare; }
        if(prevCompare != nullptr) { delete prevCompare; }

        if(!CompressWithBenchmark() && !CompressWithPrevious() && !CompressWithOther())
        {
            info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
        }

    } else if(info->compressionLevel == 2) {

        //Compare with previous and with benchmark
        //Delete if equal with benchmark or previous
        if((info->testResult.benchmark != nullptr) && !info->benchmarkOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull())
        {
            info->testResult.benchmark->compareResult =
                    info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName);
        }

        if((info->testResult.previous != nullptr) && !info->previousOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull())
        {
            info->testResult.previous->compareResult =
                    info->comparator->Compare(info->previousOutputFullFolderName, info->outputFullFolderName);
        }

        int *benchmarkCompare = nullptr;
        if(info->testResult.benchmark != nullptr) {
            benchmarkCompare = new int;
            *benchmarkCompare = info->testResult.benchmark->compareResult;
        }

        int *prevCompare = nullptr;
        if(info->testResult.previous != nullptr) {
            prevCompare = new int;
            *prevCompare = info->testResult.previous->compareResult;
        }

        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                   info->consoleOutput, info->outputFullFolderName, benchmarkCompare, prevCompare, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                   outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        if(benchmarkCompare != nullptr) { delete benchmarkCompare; }
        if(prevCompare != nullptr) { delete prevCompare; }

        if(!CompressWithBenchmark() && !CompressWithPrevious())
        {
            info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
        }

    } else if(info->compressionLevel == 1) {

        //Not compare with previous
        if(info->testResult.previous != nullptr) {
            delete info->testResult.previous;
            info->testResult.previous = nullptr;
        }

        //Compare only with benchmark
        //Delete if equal with benchmark
        if((info->testResult.benchmark != nullptr) && !info->benchmarkOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull())
        {
            info->testResult.benchmark->compareResult =
                    info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName);
        }

        int *benchmarkCompare = nullptr;
        if(info->testResult.benchmark != nullptr) {
            benchmarkCompare = new int;
            *benchmarkCompare = info->testResult.benchmark->compareResult;
        }

        int outStatusIndex;
        QString testStatusDescription;
        info->comparator->CalculateStatusIndex(info->testResult.exitStatus, info->testResult.exitCode, info->testResult.execTimeMs,
                   info->consoleOutput, info->outputFullFolderName, benchmarkCompare, nullptr, ITestOutputComparator::BenchmarkStatus::NO_BENCHMARK,
                   outStatusIndex, info->testResult.comment);

        info->comparator->GetTestStatus(outStatusIndex, info->testResult.label, info->testResult.color, testStatusDescription);

        if(benchmarkCompare != nullptr) { delete benchmarkCompare; }

        if(!CompressWithBenchmark())
        {
            info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
        }
    }

theEnd:
    delete proc;
    emit finished(info);
}

bool TestExecutor::CompressWithBenchmark()
{
    TestCompareResult *benchmark = info->testResult.benchmark;
    if((benchmark != nullptr) && (benchmark->compareResult == 0))
    {
        info->testResult.outMark = info->testResult.benchmark->outMark;
        QDir currentOutputFolder(info->outputFullFolderName);
        currentOutputFolder.removeRecursively();
        return true;
    }
    return false;
}

bool TestExecutor::CompressWithPrevious()
{
    TestCompareResult *previous = info->testResult.previous;
    if((previous != nullptr) && (previous->compareResult == 0))
    {
        info->testResult.outMark = info->testResult.previous->outMark;
        QDir currentOutputFolder(info->outputFullFolderName);
        currentOutputFolder.removeRecursively();
        return true;
    }
    return false;
}

bool TestExecutor::CompressWithOther()
{
    for(int i = 1; i < info->otherOutputFullFolderNames.count(); i++)
    {
        QString otherOutputFullFolderName = QDir(info->otherOutputFullFolderNames.at(i)).dirName();

        if(!otherOutputFullFolderName.isNull() && !info->outputFullFolderName.isNull()
        && (info->comparator->Compare(otherOutputFullFolderName, info->outputFullFolderName) == 0))
        {
            info->testResult.outMark = otherOutputFullFolderName;
            QDir currentOutputFolder(info->outputFullFolderName);
            currentOutputFolder.removeRecursively();
            return true;
        }
    }
    return false;
}
