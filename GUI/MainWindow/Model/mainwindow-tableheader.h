#ifndef MAINWINDOWTABLEHEADER_H
#define MAINWINDOWTABLEHEADER_H

#include <QString>
#include "Common/DBManager/run-description.h"

struct MainWindowTableHeader
{
    QString name;
    RunDescription *runDescription;
};

#endif // MAINWINDOWTABLEHEADER_H
