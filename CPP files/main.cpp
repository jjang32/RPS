#include "mainwindow.h"
#include <QApplication>

//MAKE BUTTONS REALLY CLOSE TOGETHER WHEN ALLOWING CHOICE OF RPS

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();
    return a.exec();
}
