#include "componentlist.h"

#include "global.h"
#include "fileio/miscfiles.h"
#include "fileio/xml.h"

#include <QDirIterator>
#include <QHeaderView>
#include <QPainter>
#include <QTransform>

// TODO remove
#include <iostream>
#include <QDebug>
#include <QFileSystemModel>

#include "ui/objects/gobject.h"

ComponentList::ComponentList(QWidget *parent)
    : QTreeView(parent), _model(new QStandardItemModel)
{
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    header()->hide();
    synchronize();
    setModel(_model);
}

void ComponentList::iterate(const QString &dir, QStandardItem *parent)
{
    QDir _dir(dir, "", QDir::DirsLast | QDir::Name, QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    for (auto file : _dir.entryInfoList())
    {
        if (file.isDir())
        {
            auto *dirItem = createDir();
            iterate(file.absoluteFilePath(), dirItem);
            parent->appendRow(dirItem);
        }
        else if (file.fileName() == "!meta")
            parent->setText(readMeta(file.absolutePath()));
        else
            parent->appendRow(createItem(file.absoluteFilePath()));
    }
}

void ComponentList::synchronize()
{
    auto *rootItem = createDir();
    iterate(systemSymbolPath, rootItem);
    _model->appendRow(rootItem);
}

QStandardItem *ComponentList::createItem(const QString &path)
{
    // A dummy scene used just for painting the item to a pixmap
    QGraphicsScene *scene = new QGraphicsScene;
    // Parse the object and create a corresponding GObject
    Object *obj = xmlParseObject(path);
    GObject *gobj = GObject::assign(obj);
    scene->addItem(gobj);

    // Scale gobj to fit inside a 50x50 canvas
    //TODO this doesn't always work -- fix it
    auto rect = gobj->boundingRect();
    int maxDim = qMax(rect.width(), rect.height());
    gobj->setTransform(QTransform::fromScale(50. / maxDim, 50. / maxDim), true);

    scene->setSceneRect({});

    QPixmap image(50, 50);
    image.fill(Qt::white);
    QPainter painter(&image);
    scene->render(&painter);
    QStandardItem *item = new QStandardItem(image, xmlPeekName(path));
    item->setEditable(false);

    item->setSelectable(true);

    return item;
}

QStandardItem *ComponentList::createDir(const QString &name)
{
    QStandardItem *item = new QStandardItem(name);
    item->setEditable(false);

    item->setSelectable(false);

    return item;
}
