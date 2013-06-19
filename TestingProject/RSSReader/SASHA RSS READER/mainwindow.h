#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QXmlStreamReader>
#include <QHttp>
#include <QWidget>
#include <QBuffer>
#include <QModelIndex>
#include <QSystemTrayIcon.h>

class QWebView;
QT_BEGIN_NAMESPACE
class QLineEdit;
class QTreeWidget;
class QSqlTableModel;
class QTreeWidgetItem;
class QPushButton;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool deleteMaybe();
    bool createActions();
    bool createTrayIcon();
    void closeEvent(QCloseEvent *event);
   // void setVisible(bool visible);
QString SelectedIdFromFirstTable;

protected:
    void changeEvent(QEvent *e);

public:

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    Ui::MainWindow *ui;
    QHttp http;
    int connectionId;
    void parseXml();
        QString currentTag;
        QSqlTableModel *t1;
        QSqlTableModel *t2;
        QString str1, st1;
        QString str2, st2;
         QString s;
        QString buffer;
        QXmlStreamReader xml;

public slots:

    bool on_delete_2_clicked();
    void on_tableView_2_clicked(QModelIndex index);
    bool timeout();
    bool on_pushButton_clicked();
    void on_tableView_clicked(QModelIndex index);
    void readData(const QHttpResponseHeader &);

   // void finished(int id, bool error);

};

#endif // MAINWINDOW_H
