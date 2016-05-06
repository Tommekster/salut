#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //Helper helper;
    //QTimer::singleShot(0, &helper, SLOT(processArgs()));

    w.show();

    return a.exec();
}
