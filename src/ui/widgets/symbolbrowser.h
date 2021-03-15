#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include "fileio/database.h"

#include <QStandardItemModel>
#include <QTreeView>

/**
 * @brief The symbol browser, usually found in a dock widget in the main window.
 *
 * Subclasses a `QTreeView` and uses a `Database` (tree) model.
 */
class SymbolBrowser : public QTreeView
{
public:
    explicit SymbolBrowser(QWidget *parent = nullptr);

    Database *model();
};

#endif // COMPONENTLIST_H
