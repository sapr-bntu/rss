#include "mainwindow.h"
#include <QDebug>
#include "QFile"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //Включаем наш QML
    ui = new QDeclarativeView;
    db = QSqlDatabase::addDatabase("QSQLITE");
    QDir file;
    model = new TreeModel;

    if(QFile::exists(file.absolutePath()+"/rssFeeds.s3db"))
    {
        db.setDatabaseName(file.absolutePath()+"/rssFeeds.s3db");
        QObject::connect(model,SIGNAL(quite()),this,SLOT(on_TreeModel_quite()));
        if (db.open())
        {
            QSqlQuery *query = new QSqlQuery(db);
            if (!query->exec("select * from sqlite_master where type = 'table' and tbl_name = 'category'"))
            {
                query->exec("CREATE TABLE [category] ([Category] VARCHAR (0, 50))");
                query->exec("insert into category values ('World')");
                query->exec("insert into category values ('Europe')");
                query->exec("insert into category values ('Politics')");
                query->exec("insert into category values ('Business')");
                query->exec("insert into category values ('Technology')");
                query->exec("insert into category values ('Entertainment')");
                query->exec("insert into category values ('Health')");
                query->exec("insert into category values ('Sciense')");
                query->exec("insert into category values ('Sport')");
                query->exec("insert into category values ('Other')");
            }
            if (!query->exec("select * from sqlite_master where type = 'table' and tbl_name = 'main'"))
            {
                query->exec("CREATE TABLE [main] ([feed] VARCHAR (80), [Category] VARCHAR (20), [Name] VARCHAR (15), PRIMARY KEY ([feed], [Name]))");
            }
            model->SetDataBase(db);
            model->AddFeedsItem();
            model->AddFeeds();
        }
    }
    else
    {
        db.setDatabaseName(file.absolutePath()+"/rssFeeds.s3db");
        QObject::connect(model,SIGNAL(quite()),this,SLOT(on_TreeModel_quite()));

        if (db.open())
        {
            QSqlQuery *query = new QSqlQuery(db);
            query->exec("CREATE TABLE [category] ([Category] VARCHAR (0, 50))");
            query->exec("CREATE TABLE [main] ([feed] VARCHAR (80), [Category] VARCHAR (20), [Name] VARCHAR (15), PRIMARY KEY ([feed], [Name]))");
            query->exec("insert into category values ('World')");
            query->exec("insert into category values ('Europe')");
            query->exec("insert into category values ('Politics')");
            query->exec("insert into category values ('Business')");
            query->exec("insert into category values ('Technology')");
            query->exec("insert into category values ('Entertainment')");
            query->exec("insert into category values ('Health')");
            query->exec("insert into category values ('Sciense')");
            query->exec("insert into category values ('Sport')");
            query->exec("insert into category values ('Other')");
            model->SetDataBase(db);
            model->AddFeedsItem();
            model->AddFeeds();
        }
    }
    ui->setSource(QUrl("qrc:/Qml/main.qml"));
    Root = ui->rootObject();
    ui->rootContext()->setContextProperty("treeModel", model);
    ui->rootContext()->setContextProperty("window",this);
    setCentralWidget(ui);
    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);
}

void MainWindow::on_TreeModel_quite()
{
    this->close();
}

void MainWindow::deleteFeed()
{
    QObject *feedName = Root->findChild<QObject*>("nameinput");
    QString str=(feedName->property("text")).toString();
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("Delete from main where Name = '"+str+"'");
    if(query->numRowsAffected()==-1)
        feedName->setProperty("text","Failed");
    else
        feedName->setProperty("text","Sucess. Feed deleted");
    model->ReInitFeeds();
}
void MainWindow::addCategory()
{
   QObject *feedName = Root->findChild<QObject*>("nameinput1");
    QString str=(feedName->property("text")).toString();
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("INSERT INTO category (Category) "
                "VALUES ('"+str+"')");
    if(query->numRowsAffected()==-1)
        feedName->setProperty("text","Failed");
    else
        feedName->setProperty("text","Sucess. Name added");
    model->ReInitFeeds();
}
void MainWindow::deleteCategory()
{
    QObject *feedName = Root->findChild<QObject*>("nameinput2");
    QString str=(feedName->property("text")).toString();
    QSqlQuery *query = new QSqlQuery(db);
    query->exec("Delete from category where Category = '"+str+"'");

    if(query->numRowsAffected()==-1)
        feedName->setProperty("text","Failed");
    else
        feedName->setProperty("text","Sucess. Category deleted");
    QSqlQuery *query1 = new QSqlQuery(db);
    query1->exec("Delete from main where Category = '"+str+"'");


    if(query->numRowsAffected()==-1)
        feedName->setProperty("text","Failed");
    else
        feedName->setProperty("text","Sucess. Feed,Category deleted");
    model->ReInitFeeds();
}
void MainWindow::addFeed()
{
    QObject *feed = Root->findChild<QObject*>("feedinput");
    QString feedStr=(feed->property("text")).toString();
    feed = Root->findChild<QObject*>("feedNameinput");
    QString feedNameStr=(feed->property("text")).toString();
    feed = Root->findChild<QObject*>("feedCategoryinput");
    QString categoryStr = (feed->property("text")).toString();

    QSqlQuery *query = new QSqlQuery(db);
      query->exec("INSERT INTO main (feed,Category,Name ) "
                "VALUES ('"+feedStr+"','"+categoryStr+"','"+feedNameStr+"')");
    feed=Root->findChild<QObject*>("feedNameinput");
    if(query->numRowsAffected()==-1)
        feed->setProperty("text","Failed");
    else
        feed->setProperty("text","Sucess. Feed added");
    model->ReInitFeeds();
}

MainWindow::~MainWindow()
{
    //Удаляем QML
    delete ui;
}


