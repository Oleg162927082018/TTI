#include "UI/MainWindow/View/mainwindow.h"
#include <QApplication>

#include "Common/DllManager/dllmanager.h"
#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
