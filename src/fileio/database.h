#ifndef DATABASE_H
#define DATABASE_H

#include "model/object.h"

#include <QAbstractItemModel>
#include <QString>

/**
 * @brief An item in a `Database`.
 */
class DatabaseItem
{
public:
    /**
     * @brief Construct a database item.
     * @param path The path of the symbol represented by this item.
     * @param parentItem The parent of this item in the tree.
     *
     * This will automatically resolve whether `path` points to a directory,
     * symbol file, or a !meta file.
     */
    explicit DatabaseItem(const QString &path, DatabaseItem *parentItem = nullptr);
    ~DatabaseItem();

    void appendItem(DatabaseItem *child);

    // GETTERS
    QString getName() const;
    QString getPath() const;
    /**
     * @brief Return the icon used to represent the item in the component browser.
     *
     * The icon is dynamically allocated only when it is to be displayed
     * to the user, and persists subsequently.
     */
    QImage getIcon() const;
    Object *getObject();
    Qt::ItemFlags getFlags() const;
    bool isDir() const;

    // SETTERS
    /**
     * @brief Set the path of this database item.
     */
    void setPath(QString path);
    /**
     * @brief Set the friendly name of the item.
     *
     * This will normally be loaded from the symbol file (or !meta file for
     * directories), but it can be changed manually as well.
     */
    void setName(const QString &name);

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

/**
 * @brief A database of symbols.
 *
 * The database is represented by a tree model.
 */
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
    /**
     * @brief Update the database from the file system.
     */
    void update();
    /**
     * @brief Iterate through all leaves of the database tree.
     * @param index The index from which to start
     * @param fun Function to execute for each leaf
     * @return If the method is called recursively, this determines if
     * the recursion should be terminated immediately after processing
     * the first leaf.
     */
    bool iterateLeaves(const QModelIndex &index,
                 std::function<bool(const QModelIndex&)> fun);

    // BOILERPLATE
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = {}) const
        override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

private:
    // HELPERS
    /**
     * @brief Recursively add all subdirectories and subfiles to `parent`.
     *
     * This will iterate through the entire directory tree inside `dir` and form
     * a corresponding tree in the model. `parent` is the database item that
     * represents the given directory `dir`.
     */
    void iterate(const QString &dir, DatabaseItem *parent);

    // ATTRIBUTES
    DatabaseItem *rootItem{};
    QString path;
};

#endif // DATABASE_H
