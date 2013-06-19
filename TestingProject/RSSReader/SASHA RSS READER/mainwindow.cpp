#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QtCore>
#include <QtGui>
#include <QtWebKit>
#include <QtNetwork>
#include <QtSql>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

   createActions();
   createTrayIcon();
   trayIcon->show();
   deleteMaybe();
    QTimer *timer = new QTimer(this);
         connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
         timer->start(300000);

    connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)), this, SLOT(readData(const QHttpResponseHeader &)));
  //  connect(&http, SIGNAL(requestFinished(int, bool)), this, SLOT(finished(int, bool)));
    t1 = new QSqlTableModel(this);
    t1->setTable("rss");
    t1->setEditStrategy(QSqlTableModel::OnManualSubmit);
    t1->select();
    t2 = new QSqlTableModel(this);
    t2->setTable("news");
    t2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    t2->select();

    ui->tableView->setModel(t1);
    ui->tableView_2->setModel(t2);
    ui->tableWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool MainWindow::timeout()
{
    try
    {
        QSqlQuery query;
        for (int i=0; i<t1->rowCount(); ++i){
        query.exec("Select rssurl from rss where idrss = '"+QString(i)+"'");
    xml.clear();
    QUrl url(t1->record(i).value("rssurl").toString());
    http.setHost(url.host());
    connectionId = http.get(url.path());
    qWarning("END CLICKED TABLE");
    return true;
        }
    }
        catch(int i)
        {
            return false;
        }
}

void MainWindow::readData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() != 200)
    {
        http.abort();
    }
        else {
        xml.addData(http.readAll());
        parseXml();
    }
}

void MainWindow::parseXml()
{
    QSqlQuery query;
    QString string;
    bool f = false;

    while (!xml.atEnd())
    {

        xml.readNext();
        int i = t2->rowCount();
        query.exec("Select news from news where idnews = '"+QString(i)+"'");
        string = t2->record(i).value("news").toString();
        if (xml.isStartElement()){currentTag = xml.name().toString();}
        else if (xml.isEndElement()){}
        else if (xml.isCharacters() && !xml.isWhitespace()){
            if (currentTag == "title"){
                f = false;
                st1 = xml.text().toString();
                if (string == st1)
                    goto m1;
                if(str1!=st1)
                {
                str1 = st1;
                f = true;}}
            else if (currentTag == "link"){
                f = false;
                st2 = xml.text().toString();
                if (str2 != st2)
                {f = true;
            str2 = st2;}}
            if (str1 != NULL && str2 != NULL && f==true){
            f=false;
            query.exec("insert into news (idrss, urlnews, news) values(1,'"+str2+"', '"+str1+"')");
            t2->select();
            t2->revertAll();
            //qDebug()<<"-------->>>>>  "<<str2<<"     ++++       "<<str1;
        }}
    m1: ;
        //qWarning("New NEWS is not found...");
    }
    if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError){
        qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
        http.abort();}
}

void MainWindow::on_tableView_clicked(QModelIndex index)
{
//    QSqlQuery query;
//    int i = (int)index.row() ;
//    QString asd;
//
////    int i = (int)index.row();
//    query.exec("Select rssurl from rss where idrss = '"+asd.setNum(i)+"'");
////    query.exec("Select rssurl from rss where idrss = '"+QString(i)+"'");
//xml.clear();
//QUrl url(t1->record(i).value("rssurl").toString());
//http.setHost(url.host());
//connectionId = http.get(url.path());
//
////query.exec("Select * from news where idrss = '"+QString(i)+"'");
////
////for (int i=1; 1<t2->rowCount();++i){
////ui->tableWidget->cellWidget(i,1)->
////ui->tableWidget->cellWidget(i,2)->data
////    }

 int asd = ui->tableView->currentIndex().row();

    SelectedIdFromFirstTable = t1->data(t1->index(asd,0)).toString();
    ui->delete_2->setEnabled(true);

}

bool MainWindow::on_pushButton_clicked()
{
    try
    {
    ui->delete_2->setEnabled(false);
    QSqlQuery query;

    query.exec("insert into rss(rssurl,rssname) values('"+ui->lineEdit1->text()+"', '"+ui->lineNameRss->text()+"')");

         t1->select();
         return true;
    }
    catch(int i)
    {
        return false;
    }
}

void MainWindow::on_tableView_2_clicked(QModelIndex index)
{
    ui->delete_2->setEnabled(false);

    int i = (int)index.row() +1 ;
    s=t2->record(i).value("urlnews").toString();

    ui->webView->load(QUrl(s));

    deleteMaybe();
}
////////////////////////////////////////////////////

bool MainWindow::deleteMaybe()
{


    QSqlQuery query;
   query.exec("SELECT COUNT() FROM news  WHERE  was_read != '0' ");

   query.first();
   if (query.value(0).toInt() == 0){
       trayIcon->setIcon(QIcon("..\\1.png"));
       trayIcon->show();
       return true;
   }else{
       trayIcon->setIcon(QIcon("..\\2.png"));
       trayIcon->show();
       return false;
   }

   qDebug()<<query.value(0).toInt();
}


void MainWindow::closeEvent(QCloseEvent *event)
 {
    ui->delete_2->setEnabled(false);
    qDebug()<< "closeEventOpened";

//    trayIcon->setIcon(QIcon("D:\\1.png"));
//    trayIcon->show();
    if (trayIcon->isVisible()) {
         QMessageBox::information(this, tr("Systray"),
                                  tr("The program will keep running in the "
                                     "system tray. To terminate the program, "
                                     "choose <b>Quit</b> in the context menu "
                                     "of the system tray entry."));
         hide();
        event->ignore();
     }
 }

bool MainWindow::createTrayIcon()
 {
    qDebug()<< "createIconOpened";
    try
    {
     trayIconMenu = new QMenu(this);
     trayIconMenu->addAction(minimizeAction);
     trayIconMenu->addAction(maximizeAction);
     trayIconMenu->addAction(restoreAction);
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);

     //trayIcon = new QSystemTrayIcon(this);
      trayIcon= new QSystemTrayIcon(QIcon("..\\1.png"),this);
     trayIcon->setContextMenu(trayIconMenu);
     return true;
    }
    catch(int i)
    {
        return false;
    }

 }
bool MainWindow::createActions()
 {
    try
    {
    qDebug()<< "1111111111111";
     minimizeAction = new QAction(tr("Mi&nimize"), this);
     connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

     maximizeAction = new QAction(tr("Ma&ximize"), this);
     connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

     restoreAction = new QAction(tr("&Restore"), this);
     connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

     quitAction = new QAction(tr("&Quit"), this);
     connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
     return true;
    }
    catch(int i)
    {
        return false;
    }
 }
//void MainWindow::setVisible(bool visible)
// {
//     minimizeAction->setEnabled(visible);
//     maximizeAction->setEnabled(!isMaximized());
//     restoreAction->setEnabled(isMaximized() || !visible);
//     //QDialog::setVisible(visible);
//     QMainWindow::setVisible(visible);
// }

bool MainWindow::on_delete_2_clicked()
{ try
    {
    QSqlQuery query;
   query.exec("DELETE  FROM rss  WHERE  idrss = '"+ SelectedIdFromFirstTable +"' ");

       t1->select();
       return true;
    }
    catch(int i)
    {
        return false;
    }
}

