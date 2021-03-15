#include "database.h"

#include "global.h"
#include "fileio/miscfiles.h"
#include "xml.h"
#include "ui/objects/gobject.h"

#include <QDir>
#include <QFile>
#include <QGraphicsScene>
#include <QImage>
#include <QApplication>

DatabaseItem::DatabaseItem(const QString &path, DatabaseItem *parentItem)
    : parent(parentItem)
{
    setPath(path);
}

DatabaseItem::~DatabaseItem()
{
    qDeleteAll(childItems);
    delete object;
    delete icon;
}

void DatabaseItem::appendItem(DatabaseItem *child)
{
    childItems.append(child);
    child->parent = this;
}

// GETTERS

QString DatabaseItem::getName() const
{
    return name;
}

QString DatabaseItem::getPath() const
{
    return path;
}

QImage DatabaseItem::getIcon() const
{
    if (icon != nullptr) // An up-to-date icon has already been generated
        return *icon;

    if (object == nullptr)
        // The object is not a component, but a directory
        return {};

    // Create a scene that we will use to generate the icon
    QGraphicsScene *scene = new QGraphicsScene;
    GObject *gobject = GObject::assign(object);
    scene->addItem(gobject);
    gobject->setCosmetic(true);

    auto rect = gobject->boundingRect();
    float maxDim = qMax(rect.width(), rect.height());
    // The icon will be iconSize x iconSize
    float iconSize = 30;

    // Center the object in the icon...
    if (maxDim != 0)
    {
        // Auto-adjust scene
        float scale = iconSize / maxDim; // We must scale the gobject by this amount
        gobject->setTransform(QTransform::fromScale(scale, scale), true);
        scene->setSceneRect({}); // Adjust the scene to tightly fit the object

        auto sceneRect = scene->sceneRect();
        // Center the object in the scene and resize scene to icon size
        sceneRect.setTopLeft(sceneRect.topLeft() + QPointF{(scale * rect.width() - iconSize) / 2, (scale * rect.height() - iconSize) / 2});
        sceneRect.setSize({iconSize, iconSize});
        scene->setSceneRect(sceneRect);
    }

    // Create the icon and finalize

    icon = new QImage(iconSize, iconSize, QImage::Format_ARGB32);
    icon->fill({0,0,0,0});
    QPainter painter(icon);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    scene->render(&painter);

    return *icon;
}

Object *DatabaseItem::getObject()
{
    return object;
}

Qt::ItemFlags DatabaseItem::getFlags() const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (object != nullptr)
        flags |= Qt::ItemIsDragEnabled;
    return flags;
}

bool DatabaseItem::isDir() const
{
    return object == nullptr;
}

// SETTERS

void DatabaseItem::setPath(QString path)
{
    if (path != this->path)
    {
        this->path = path;
        path = resolvePath(path);
        delete object;
        if (QFileInfo(path).isDir())
            object = nullptr;
        else
        {
            setName(xmlPeekName(path));
            object = (CompositeObject *) xmlParseObject(path);
            object->setFileName(this->path);
        }
        // Invalidate the icon - it will have to be generated again when required
        icon = nullptr;
    }
}

void DatabaseItem::setName(const QString &name)
{
    this->name = name;
}

// BOILERPLATE

DatabaseItem *DatabaseItem::getChild(int row)
{
    if (row < 0 || row >= getChildCount())
        return nullptr;
    return childItems[row];
}

int DatabaseItem::getChildCount() const
{
    return childItems.count();
}

int DatabaseItem::getColumnCount() const
{
    return 1;
}

QVariant DatabaseItem::getData(int column) const
{
    if (column == 0)
        return name;
    else
        return {};
}

int DatabaseItem::getRow() const
{
    if (parent)
        return parent->childItems.indexOf(const_cast<DatabaseItem*>(this));
    return 0;
}

DatabaseItem *DatabaseItem::getParentItem()
{
    return parent;
}

/***********
* Database *
 ***********/

Database::Database(const QString &path)
    : path(path)
{
    update();
}

Database::~Database()
{
    delete rootItem;
}

QVariant Database::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    DatabaseItem *item = static_cast<DatabaseItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
        return item->getData(index.column());
    else if (role == Qt::DecorationRole)
        return item->getIcon();
    else if (role == Qt::ToolTipRole)
        return resolvePath(item->getPath());
    else return {};
}

void Database::update()
{
    // Delete the root item and recursively add the directory from scratch
    if (rootItem)
        delete rootItem;
    rootItem = new DatabaseItem(path);
    iterate(resolvePath(path), rootItem);
}

bool Database::iterateLeaves(const QModelIndex &ind,
                             std::function<bool (const QModelIndex &)> fun)
{
     if (ind.isValid() && !fun(ind))
         return false;

     if (!hasChildren(ind) || (ind.flags() & Qt::ItemNeverHasChildren))
          return true;
     for (int i = 0; i < rowCount(ind); ++i)
         if (!iterateLeaves(index(i, 0, ind), fun))
             return false;
     return true;
}

// BOILERPLATE

Qt::ItemFlags Database::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    else
    {
        DatabaseItem *item = static_cast<DatabaseItem*>(index.internalPointer());
        return item->getFlags();
    }
}

QModelIndex Database::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};
    DatabaseItem *parentItem;

    if (!parent.isValid()) // Parent is root item
        parentItem = rootItem;
    else // Parent is not root item
        parentItem = static_cast<DatabaseItem*>(parent.internalPointer());

    DatabaseItem *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return {};
}

QModelIndex Database::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {}; // Root item is considered its parent

    DatabaseItem *childItem = static_cast<DatabaseItem*>(index.internalPointer());
    DatabaseItem *parentItem = childItem->getParentItem();

    if (parentItem == rootItem)
        return {};
    else
        return createIndex(parentItem->getRow(), 0, parentItem);
}

int Database::rowCount(const QModelIndex &parent) const
{
    DatabaseItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<DatabaseItem*>(parent.internalPointer());

    return parentItem->getChildCount();
}

int Database::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<DatabaseItem*>(parent.internalPointer())->getColumnCount();
    else
        return rootItem->getColumnCount();
}

// HELPER METHODS

void Database::iterate(const QString &dir, DatabaseItem *parent)
{
    QDir _dir(dir, "", QDir::DirsLast | QDir::Name,
              QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    // For each entry (file or directory) in `dir`
    foreach (auto entry, _dir.entryInfoList())
    {
        // Test if `entry` is a file or directory
        if (entry.isDir())
        { // It is
            // Form an item that corresponds to the directory
            auto *dirItem = new DatabaseItem(entry.absoluteFilePath()); // which path to show here?
            // Recurse into the directory
            iterate(entry.absoluteFilePath(), dirItem);
            // Add the directory to its parent in the model
            parent->appendItem(dirItem);
        }
        else if (entry.fileName() == "!meta")
            // Entry is a meta file that contains info about its directory
            parent->setName(readMeta(entry.absolutePath()));
        else
            // Normal file - add it as an item in the tree
            parent->appendItem(new DatabaseItem(entry.absoluteFilePath()));
    }
}
