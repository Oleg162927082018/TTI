#include "runmanager.h"
#include "testexecutor.h"

#include <QThread>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QDir>

#include <windows.h>

TestExecutor::TestExecutor(TestInfo *info) : QObject(0)
{
    this->info = info;
}

void TestExecutor::doKill()
{
    if(proc != NULL) { proc->kill(); }
}

void TestExecutor::run()
{
    /*SHELLEXECUTEINFO shExecInfo;
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

    shExecInfo.fMask = NULL;
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = L"runas";
    shExecInfo.lpFile = L"D:/Home/Total Test Integrator/lib/MLFW-PhantomJS/test-ready/phantomjs.exe";
    shExecInfo.lpParameters = L"\"D:/Home/Total Test Integrator/lib/MLFW-PhantomJS/test-ready/scripts/OpenStartPage/OpenStartPage.js\" http://ya.ru \"D:/temp\"";
    shExecInfo.lpDirectory = NULL;
    shExecInfo.nShow = SW_HIDE;
    shExecInfo.hInstApp = NULL;

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
        info->testResult.exitStatus = "not-started";
        info->testResult.exitCode = -1;
        info->testResult.status = "Not started!";
        info->testResult.color = QColor(128,128,128);
        info->testResult.benchmarkCompareResult = -1;
        info->testResult.previousCompareResult = -1;

        info->consoleOutput.append("Test not started:");
        info->consoleOutput.append(info->exeCommand);
        info->consoleOutput.append(info->argumentList);
        info->consoleOutput.append(proc->errorString());
    }
    else
    {
        proc->closeWriteChannel();

        if (!proc->waitForFinished(info->waitingTime * 1000))
        {
            info->testResult.execTimeMs = info->waitingTime * 1000;
            info->testResult.exitStatus = "timeout-expired";
            info->testResult.exitCode = -1;
            info->testResult.status = "Timeout expired!";
            info->testResult.color = QColor(0,0,255);
            info->testResult.benchmarkCompareResult = -1;
            info->testResult.previousCompareResult = -1;

            info->consoleOutput.append("Timeout expired:");
            info->consoleOutput.append(info->exeCommand);
            info->consoleOutput.append(info->argumentList);
        }
        else
        {
            info->testResult.execTimeMs = QDateTime::currentMSecsSinceEpoch() - info->testResult.execTimeMs;

            QByteArray result = proc->readAll();
            QString console = QString::fromLocal8Bit(result.data());
            info->consoleOutput.append(console.split('\n'));

            info->testResult.exitStatus = "complete";
            info->testResult.exitCode = proc->exitCode();

            //Compare results with previous, benchmark and other results
            if(info->compressionLevel == 3)
            {
                //Compare with all and delete if equal output found
                if(info->benchmarkOutputFullFolderName.isNull() || info->outputFullFolderName.isNull())
                {
                    info->testResult.benchmarkCompareResult = -1;
                }
                else { info->testResult.benchmarkCompareResult = info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName); }

                if(info->previousOutputFullFolderName.isNull() || info->outputFullFolderName.isNull())
                {
                    info->testResult.previousCompareResult = -1;
                }
                else { info->testResult.previousCompareResult = info->comparator->Compare(info->previousOutputFullFolderName, info->outputFullFolderName); }

                info->comparator->CalculateStatus(info->consoleOutput, info->outputFullFolderName,
                                                  info->testResult.exitCode, info->testResult.execTimeMs, info->testResult.benchmarkCompareResult,
                                                  info->testResult.previousCompareResult, info->testResult.status, info->testResult.color);

                if(!CompressWithBenchmark() && !CompressWithPrevious() && !CompressWithOther())
                {
                    info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
                }
            }
            else if(info->compressionLevel == 2)
            {
                //Compare with previous and with benchmark
                //Delete if equal with anything
                if(info->benchmarkOutputFullFolderName.isNull() || info->outputFullFolderName.isNull())
                {
                    info->testResult.benchmarkCompareResult = -1;
                }
                else { info->testResult.benchmarkCompareResult = info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName); }

                if(info->previousOutputFullFolderName.isNull() || info->outputFullFolderName.isNull())
                {
                    info->testResult.previousCompareResult = -1;
                }
                else { info->testResult.previousCompareResult = info->comparator->Compare(info->previousOutputFullFolderName, info->outputFullFolderName); }

                info->comparator->CalculateStatus(info->consoleOutput, info->outputFullFolderName,
                                                  info->testResult.exitCode, info->testResult.execTimeMs, info->testResult.benchmarkCompareResult,
                                                  info->testResult.previousCompareResult, info->testResult.status, info->testResult.color);

                if(!CompressWithBenchmark() && !CompressWithPrevious())
                {
                    info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
                }
            }
            else if(info->compressionLevel == 1)
            {
                //Compare with benchmark
                //Delete if equal with benchmark
                if(info->benchmarkOutputFullFolderName.isNull() || info->outputFullFolderName.isNull())
                {
                    info->testResult.benchmarkCompareResult = -1;
                }
                else { info->testResult.benchmarkCompareResult = info->comparator->Compare(info->benchmarkOutputFullFolderName, info->outputFullFolderName); }

                info->testResult.previousCompareResult = -1;

                info->comparator->CalculateStatus(info->consoleOutput, info->outputFullFolderName,
                                                  info->testResult.exitCode, info->testResult.execTimeMs, info->testResult.benchmarkCompareResult,
                                                  info->testResult.previousCompareResult, info->testResult.status, info->testResult.color);
                if(!CompressWithBenchmark())
                {
                    info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
                }
            }
            else if(info->compressionLevel == 0)
            {
               //Not compare with anything
               info->testResult.benchmarkCompareResult = -1;
               info->testResult.previousCompareResult = -1;
               info->comparator->CalculateStatus(info->consoleOutput, info->outputFullFolderName,
                                                 info->testResult.exitCode, info->testResult.execTimeMs, info->testResult.benchmarkCompareResult,
                                                 info->testResult.previousCompareResult, info->testResult.status, info->testResult.color);

               info->testResult.outMark = QDir(info->outputFullFolderName).dirName();
            }
        }
    }

    delete proc;
    emit finished(info);
}

bool TestExecutor::CompressWithBenchmark()
{
    if(info->testResult.benchmarkCompareResult == 0)
    {
        info->testResult.outMark = info->testResult.benchmarkOutMark;
        QDir currentOutputFolder(info->outputFullFolderName);
        currentOutputFolder.removeRecursively();
        return true;
    }
    return false;
}

bool TestExecutor::CompressWithPrevious()
{
    if(info->testResult.previousCompareResult == 0)
    {
        info->testResult.outMark = info->testResult.previousOutMark;
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
