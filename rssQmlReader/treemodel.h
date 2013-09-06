#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QtDeclarative/QDeclarativeView>
#include <QGraphicsObject>
#include <QtGui>
#include <QDeclarativeContext>
#include <QSqlTableModel>

class TreeModelItem;
class TreeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TreeModel(QObject *parent=0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void SetDataBase(QSqlDatabase db);
    void AddFeeds();
    void AddFeedsItem();
    void ReInitFeeds();
    //Q_INVOKABLE void deleteFeed();
public slots:
    void openItem(int numIndex);
    void closeItem(int numIndex);
signals:
    void quite();
private:
    Q_DISABLE_COPY(TreeModel);
    QList<TreeModelItem *> items;
    QSqlDatabase inpDB;
    enum ListMenuItemRoles
    {

        NameRole = Qt::UserRole+1,
        FeedRole,
        LevelRole,
        IsOpenedRole,
        HasChildrenRole
    };
};

#endif // TREEMODEL_H

