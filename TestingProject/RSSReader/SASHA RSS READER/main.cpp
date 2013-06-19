#include <QtGui/QApplication>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();

    if (createConnection())
    {
    qWarning("The usage of QHttp is not recommended anymore, please use QNetworkAccessManager.");
    MainWindow w;
    w.show();
    return a.exec();
    //rsslisting->show();
    //return app.exec();
}else {
    return a.exec();
}
}
