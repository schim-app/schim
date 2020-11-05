#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include "fileio/database.h"

#include <QStandardItemModel>
#include <QTreeView>

class ComponentList : public QTreeView
{
public:
    ComponentList(QWidget *parent = nullptr);

    Database *model();
};

#endif // COMPONENTLIST_H
