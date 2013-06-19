#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDeclarative/QDeclarativeView>
#include <QGraphicsObject>
#include <QtGui>
#include <QDeclarativeContext>
#include <QSqlDatabase>
#include <QDir>
#include "treemodel.h"
#include <QSqlTableModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Q_INVOKABLE void deleteFeed();
    Q_INVOKABLE void addFeed();
     Q_INVOKABLE void addCategory();
     Q_INVOKABLE void deleteCategory();

public slots:
    void on_TreeModel_quite();
private:
    QSqlDatabase db;
    QDeclarativeView *ui;
    QObject *Root;
    TreeModel *model;
};




#endif // MAINWINDOW_H
