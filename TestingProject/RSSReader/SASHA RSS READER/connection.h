#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../base.s3db");
    if (!db.open())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("ERROR"), QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;
       query.exec("insert into rss values(1, 'http://bash.im/rss/', 'Bash org')");
        query.exec("insert into rss values(2, 'http://rss.interfax.by/if_news_belarus.rss', 'Interfax')");
        //query.exec("delete from rss");
    return true;
}


#endif // CONNECTION_H
