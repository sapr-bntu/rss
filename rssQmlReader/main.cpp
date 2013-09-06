#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(800,500);
    w.setMaximumSize(800,500);

    w.show();
    return a.exec();
}
