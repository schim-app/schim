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
    : QTreeView(parent)
{
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    //header()->hide();
    //TODO this is just a test database
    setModel(new Database(systemSymbolPath)); // TODO create this database elsewhere
}
Database *ComponentList::model()
{
    return static_cast<Database*>(QTreeView::model());
}
