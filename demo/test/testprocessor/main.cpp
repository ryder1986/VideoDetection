#include "mainwindow.h"
#include <QApplication>
#include "tool.h"
int main(int argc, char *argv[])
{
    prt(info,"start");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
