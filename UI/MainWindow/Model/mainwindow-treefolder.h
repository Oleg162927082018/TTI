#ifndef MAINWINDOWTREEFOLDER_H
#define MAINWINDOWTREEFOLDER_H

#include <Common/DBManager/test-case.h>
#include <UI/MainWindow/Model/mainwindow-tableitem.h>
#include <UI/MainWindow/Model/mainwindow-tableheader.h>

#include <QString>
#include <QList>
#include <QMap>

struct MainWindowTreeFolder
{
    int checkState = Qt::Checked;
    bool expanded = false;
    QString name;

    MainWindowTreeFolder *parentFolder = nullptr;
    QList<MainWindowTreeFolder *> subFolders;

    TestCase *ownerTestCase = nullptr;
    QMap<int, MainWindowTableHeader *> *fullTableHeaders = nullptr;
    QMap<int, MainWindowTableHeader *> *visibleTableHeaders = nullptr;


    QList<MainWindowTableItem *> fullTableItems;
    QMap<int, MainWindowTableItem *> visibleTableItems;

};


#endif // MAINWINDOWTREEFOLDER_H
