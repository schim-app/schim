#include "database.h"

#include "global.h"
#include "fileio/miscfiles.h"
#include "xml.h"
#include "ui/objects/gobject.h"

#include <QDir>
#include <QFile>
#include <QGraphicsScene>
#include <QImage>

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

QImage DatabaseItem::getIcon() const
{
    if (icon != nullptr) // An up-to-date icon has already been generated
        return *icon;

    // If the icon is not generated

    if (object == nullptr)
        return {};

    // Create a scene that we will use to generate the icon
    QGraphicsScene *scene = new QGraphicsScene();
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

    icon = new QImage(iconSize, iconSize, QImage::Format_RGB32);
    icon->fill(Qt::white);
    QPainter painter(icon);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    scene->render(&painter);

    return *icon;
}

Qt::ItemFlags DatabaseItem::getFlags() const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (object != nullptr)
        flags |= Qt::ItemIsDragEnabled;
    return flags;
}

Object *DatabaseItem::getObject()
{
    return object;
}

void DatabaseItem::setPath(const QString &path)
{
    if (path != this->path)
    {
        if (QFileInfo(path).isDir())
        {
            delete object;
            object = nullptr;
        }
        else
            object = xmlParseObject(resolvePath(path));
        // Invalidate the icon - it will have to be generated again when required
        icon = nullptr;
    }
    this->path = path;
}

void DatabaseItem::setName(const QString &name)
{
    this->name = name;
}

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

QString DatabaseItem::getName() const
{
    return name;
}

DatabaseItem *DatabaseItem::getParentItem()
{
    return parent;
}

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
    if (!index.isValid())
        return {};
    DatabaseItem *item = static_cast<DatabaseItem*>(index.internalPointer());
    if (role == Qt::DisplayRole)
        return item->getData(index.column());
    else if (role == Qt::DecorationRole)
        return item->getIcon();
    else return {};
}

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

    if (!parent.isValid())
        parentItem = rootItem;
    else
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
        return {};

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

void Database::iterate(const QString &dir, DatabaseItem *parent)
{
    QDir _dir(dir, "", QDir::DirsLast | QDir::Name, QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    for (auto file : _dir.entryInfoList())
    {
        if (file.isDir())
        {
            auto *dirItem = new DatabaseItem(file.absoluteFilePath()); // which path to show here?
            iterate(file.absoluteFilePath(), dirItem);
            parent->appendItem(dirItem);
        }
        else if (file.fileName() == "!meta")
            parent->setName(readMeta(file.absolutePath()));
        else
            parent->appendItem(new DatabaseItem(file.absoluteFilePath()));
    }
}

void Database::update()
{
    if (rootItem)
        delete rootItem;
    rootItem = new DatabaseItem(path);
    iterate(path, rootItem);
}
