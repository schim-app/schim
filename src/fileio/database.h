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
    /**
     * @brief Set the path of this database item.
     */
    void setPath(QString path);
    /**
     * @brief Set the friendly name of the item.
     */
    void setName(const QString &name);

    // GETTERS
    QString getName() const;
    QString getPath() const;
    /**
     * @brief Return the icon used to represent the item in the component list.
     *
     * The icon is dynamically allocated only when it is to be displayed
     * to the user, and persists subsequently.
     */
    QImage getIcon() const;
    Object *getObject();
    Qt::ItemFlags getFlags() const;
    bool isDir() const;

    // BOILERPLATE
    DatabaseItem *getChild(int row);
    int getChildCount() const;
    int getColumnCount() const;
    QVariant getData(int column) const;
    int getRow() const;
    DatabaseItem *getParentItem();

private:
    QString path, name;
    DatabaseItem *parent;
    QVector<DatabaseItem*> childItems;
    CompositeObject *object{};
    /// Dynamically allocated by `getIcon` when needed
    mutable QImage *icon{};
};

class Database : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit Database(const QString &path);
    ~Database();

    /**
     * @brief Used by the view to display each item's data.
     *
     * Each item is displayed only with a preview icon and a name.
     */
    QVariant data(const QModelIndex &index, int role) const override;
    /** @brief Update the database from the file system. */
    void update();

    // BOILERPLATE
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

private:

    // HELPER METHODS
    /** @brief Recursively add all subdirectories and subfiles of `parent`. */
    void iterate(const QString &dir, DatabaseItem *parent);

    // ATTRIBUTES
    DatabaseItem *rootItem{};
    QString path;

};

#endif // DATABASE_H
