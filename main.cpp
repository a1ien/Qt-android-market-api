#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.login();

    w.show();

    return a.exec();
}
