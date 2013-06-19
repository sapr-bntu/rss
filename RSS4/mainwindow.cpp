#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    //Включаем наш QML
    ui = new QDeclarativeView;
    db = QSqlDatabase::addDatabase("QSQLITE");
    QDir file;
    db.setDatabaseName(file.absolutePath()+"/rssFeeds.s3db");
    model = new TreeModel;
    QObject::connect(model,SIGNAL(quite()),this,SLOT(on_TreeModel_quite()));
    if(db.open())
    {
        model->SetDataBase(db);
        model->AddFeedsItem();
        model->AddFeeds();
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


