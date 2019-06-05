#include "UI/MainWindow/View/mainwindow.h"
#include <QApplication>

#include "Common/DllManager/dllmanager.h"
#include "Common/DBManager/dbmanager.h"
#include "Common/RunManager/runmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString ver = a.applicationVersion();
    w.setWindowTitle("Total Test Integrator " + ver.mid(0,ver.indexOf('.', 2)));
    w.show();

    return a.exec();
}
