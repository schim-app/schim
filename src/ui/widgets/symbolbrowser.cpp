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

    setModel(getGlobalDatabase());
}

Database *SymbolBrowser::model()
{
    return static_cast<Database*>(QTreeView::model());
}
