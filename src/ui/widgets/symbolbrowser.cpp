#include "symbolbrowser.h"

#include "global.h"
#include "fileio/miscfiles.h"
#include "fileio/xml.h"
#include "ui/objects/gobject.h"

#include <QDirIterator>
#include <QHeaderView>
#include <QPainter>
#include <QTransform>

// TODO remove
#include <iostream>
#include <QDebug>
#include <QFileSystemModel>

SymbolBrowser::SymbolBrowser(QWidget *parent)
    : QTreeView(parent)
{
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    //TODO this is just a test database
    if (globalDatabase == nullptr)
        globalDatabase = new Database(systemSymbolPath);

    setModel(globalDatabase); // TODO create this database elsewhere
}

Database *SymbolBrowser::model()
{
    return static_cast<Database*>(QTreeView::model());
}
