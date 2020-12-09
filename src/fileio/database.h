#ifndef DATABASE_H
#define DATABASE_H

#include "model/object.h"

#include <QStandardItem>
#include <QString>

class DatabaseItem
{
public:
    explicit DatabaseItem(const QString &path, DatabaseItem *parentItem = nullptr);
    ~DatabaseItem();

    void appendItem(DatabaseItem *child);

    // SETTERS
    void setPath(QString path);
    void setName(const QString &name);

    // GETTERS
    DatabaseItem *getChild(int row);
    int getChildCount() const;
    int getColumnCount() const;
    QVariant getData(int column) const;
    int getRow() const;
    QString getName() const;
    DatabaseItem *getParentItem();
    QImage getIcon() const;
    Qt::ItemFlags getFlags() const;
    Object *getObject();

private:
    QString path, name;
    Object *object{};
    DatabaseItem *parent;
    QVector<DatabaseItem*> childItems;
    // Dynamically allocated by generateIcon when needed
    mutable QImage *icon{};
};

class Database : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Database(const QString &path);
    ~Database();

    // OVERRIDDEN GETTERS
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

    void update();

private:

    void iterate(const QString &dir, DatabaseItem *parent);
    // ATTRIBUTES

    DatabaseItem *rootItem{};
    QString path;

};

#endif // DATABASE_H
