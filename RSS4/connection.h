#ifndef CONNECTION_H
#define CONNECTION_H

#include <QDebug>
#include <QSqlDatabase>
#include <QDir>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QDir file;
    db.setDatabaseName(file.absolutePath()+"/rssFeeds.s3db");

    qDebug()<< db.databaseName();
    if (!db.open())
    {
        return false;
    }
    else
    {
        db.open();
        return true;
    }

}
#endif // CONNECTION_H
