#include "treemodel.h"
#include <QDebug>

class TreeModelItem
{
public:
    TreeModelItem(QString name_ = QString(),QString feed_ = QString())
        : name(name_),
          feed(feed_),
          level(0),
          isOpened(false)
    {}
    void adjustChildrenLevels()
    {
        foreach(TreeModelItem *item, children)
        {
            item->level = level+1;
            item->adjustChildrenLevels();
        }
    }
    QString name;
    QString feed;
    int level;
    bool isOpened;
    QList<TreeModelItem *> children;
    inline bool hasChildren() {return !children.empty();}
};

TreeModel::TreeModel(QObject *parent) :
    QAbstractListModel(parent)
{
/*
    items << new TreeModelItem("World","");
    items << new TreeModelItem("Europe","");
    items << new TreeModelItem("U.S.","");
    items << new TreeModelItem("Politics","");
    items << new TreeModelItem("Business","");
    items << new TreeModelItem("Technology","");
    items << new TreeModelItem("Entertainment","");
    items << new TreeModelItem("Health","");
    items << new TreeModelItem("Science","");
    items << new TreeModelItem("Sport","");
    items << new TreeModelItem("Other","");

   */

    QHash<int, QByteArray> roles = roleNames();
    roles.insert(NameRole, QByteArray("name"));
    roles.insert(FeedRole, QByteArray("feed"));
    roles.insert(LevelRole, QByteArray("level"));
    roles.insert(IsOpenedRole, QByteArray("isOpened"));
    roles.insert(HasChildrenRole, QByteArray("hasChildren"));
    setRoleNames(roles);
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() > (items.size()-1) )
        return QVariant();
    TreeModelItem *item = items.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case NameRole:
        return QVariant::fromValue(item->name);
    case FeedRole:
        return QVariant::fromValue(item->feed);
    case LevelRole:
        return QVariant::fromValue(item->level);
    case IsOpenedRole:
        return QVariant::fromValue(item->isOpened);
    case HasChildrenRole:
        return QVariant::fromValue(item->hasChildren());
    default:
        return QVariant();
    }
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return items.size();

}

void TreeModel::openItem(int numIndex)
{
    if (numIndex > (items.size()-1))
        return;
    if (items[numIndex]->isOpened)
        return;
    QModelIndex modelIndex = index(numIndex);
    //Выставляем флаг открытого элемента
    items[numIndex]->isOpened = true;
    //Оповещаем QML-код об изменении данных
    emit dataChanged(modelIndex, modelIndex);
    int i = numIndex+1;
    //Оповещаем QML-код о том, что будут добавлены строки в модель
    beginInsertRows(QModelIndex(), i, i+items[numIndex]->children.size()-1);
    //Добавляем всех потомков элемента в модель после этого элемента
    foreach(TreeModelItem *item, items[numIndex]->children)
        items.insert(i++, item);
    //Оповещаем QML-код о том, что все строки добавлены
    endInsertRows();
}

void TreeModel::closeItem(int numIndex)
{
    if (numIndex > (items.size()-1))
        return;
    if (!items[numIndex]->isOpened)
        return;
    QModelIndex modelIndex = index(numIndex);
    //Сбрасываем флаг открытого элемента
    items[numIndex]->isOpened = false;
    //Оповещаем QML-код об изменении данных
    emit dataChanged(modelIndex, modelIndex);
    int i = numIndex+1;
    //Ищем все элементы после текущего с большим level
    //Таким образом найдем всех прямых и косвенных потомков
    for (; i < items.size() && (items[i]->level > items[numIndex]->level); ++i) {}
    --i;
    //Оповещаем QML-код о том, что будут удалены строки из модели
    beginRemoveRows(QModelIndex(), numIndex+1, i);
    //Удаляем все посчитанные элементы из модели и сбрасываем у них флаг открытия
    while (i > numIndex)
    {
        items[i]->isOpened = false;
        items.removeAt(i--);
    }
    //Оповещаем QML-код о том, что все строки удалены
    endRemoveRows();
}

void TreeModel::SetDataBase(QSqlDatabase db)
{
    inpDB = db;
}

void TreeModel::AddFeedsItem()
{
    QSqlQueryModel *model1 = new QSqlQueryModel;
    int querySize1;
    if(!inpDB.isOpen())
    {
        inpDB.open();
    }
  QString queryText1 = "Select Category from category";
        model1->setQuery(queryText1,inpDB);
   querySize1 = model1->rowCount();
   if(querySize1>-1)
       for(int i=0;i<querySize1;i++)
      { QSqlRecord rec1 = model1-> record(i);
       items << new TreeModelItem (rec1.field(0).value().toString(),"");

   }

}
void TreeModel::AddFeeds()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    int querySize;
    if(!inpDB.isOpen())
    {
        inpDB.open();
    }
    for(int i=0;i<items.count();i++)
    {
        QString queryText = "Select feed,name from main where Category = '" + items[i]->name+"'";
        model->setQuery(queryText,inpDB);
        querySize = model->rowCount();
        if(querySize>-1)
            for(int j=0;j<querySize;j++)
            {
                QSqlRecord rec = model->record(j);
                items[i]->children<<new TreeModelItem(rec.field(1).value().toString(),rec.field(0).value().toString());
                items[i]->adjustChildrenLevels();
            }
    }
}

void TreeModel::ReInitFeeds()
{
    items.clear();
    AddFeedsItem();

    QSqlQueryModel *model = new QSqlQueryModel;

    int querySize;
    if(!inpDB.isOpen())
    {
        inpDB.open();
    }
    for(int i=0;i<items.count();i++)
    {
        items[i]->children.clear();
        QString queryText = "Select feed,name from main where Category = '" + items[i]->name+"'";
        model->setQuery(queryText,inpDB);
        querySize = model->rowCount();
        if(querySize>-1)
            for(int j=0;j<querySize;j++)
            {
                QSqlRecord rec = model->record(j);
                items[i]->children<<new TreeModelItem(rec.field(1).value().toString(),rec.field(0).value().toString());
                items[i]->adjustChildrenLevels();
            }

    }
}


