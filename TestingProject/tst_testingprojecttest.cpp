#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include  "connection.h"
#include  "mainwindow.h"
#include  "ui_mainwindow.h"

class TestingProjectTest : public QObject
{
    Q_OBJECT
    
public:
    TestingProjectTest();
    
private Q_SLOTS:


  void test_on_pushButton_clicked();
  void test_on_delete_2_clicked();
  void testCreateAction();
  void testCreateAction2();
  void testCreateTrayIcon();
  void test_on_tableView_2_clicked();
  void testCase();
  void testDeleteMaybe();


};

TestingProjectTest::TestingProjectTest()
{
}


void TestingProjectTest::test_on_pushButton_clicked()
{
    MainWindow winTest;
    QTest::mouseClick(winTest.ui->pushButton,Qt::LeftButton, 0);
     QCOMPARE(winTest.on_pushButton_clicked(),true);

}
void TestingProjectTest::test_on_delete_2_clicked()
{

    MainWindow winTest;
    QCOMPARE(winTest.on_delete_2_clicked(),true);

}
void TestingProjectTest::testCreateAction()
{
    MainWindow winTest;

    winTest.createActions();
    winTest.maximizeAction->trigger();

    QCOMPARE(winTest.isMaximized(),true);
}
void TestingProjectTest::testCreateAction2()
{
    MainWindow winTest;

    winTest.createActions();
    winTest.minimizeAction->trigger();

    QCOMPARE(winTest.isHidden(),true);
}

void TestingProjectTest::testCreateTrayIcon()
{
    MainWindow winTest;

    QCOMPARE(winTest.createTrayIcon(),true);

}

void TestingProjectTest::test_on_tableView_2_clicked()
{
   MainWindow winTest;
    QModelIndex index;
    index.child(0,0);
    winTest.ui->delete_2->setEnabled(true);
    winTest.on_tableView_2_clicked(index);
    QCOMPARE(winTest.ui->delete_2->isEnabled(),false);
}

void TestingProjectTest::testCase()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QSKIP("This test requires the SQLITE database driver", SkipAll);
    QCOMPARE(createConnection(),true);
}

void TestingProjectTest::testDeleteMaybe()
{
    MainWindow winTest;
    QCOMPARE(winTest.deleteMaybe(),true);
}

QTEST_MAIN(TestingProjectTest)


#include "tst_testingprojecttest.moc"
