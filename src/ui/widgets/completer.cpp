#include "completer.h"

// TODO rm
#include <QModelIndex>
#include <iostream>

#include "model/header.h"

// CONSTRUCTORS

Completer::Completer(QObject *parent)
    : QCompleter(parent)
{
}

Completer::Completer(Database *model, QObject *parent)
    : QCompleter(parent)
{
    auto *proxy = new CompleterProxyModel(this);
    proxy->setSourceModel(model);
    setModel(proxy);
}

Completer::~Completer()
{
    delete model();
}

CompleterProxyModel *Completer::model()
{
    return static_cast<CompleterProxyModel*>(QCompleter::model());
}

/***************
 * PROXY MODEL *
 ***************/

// HELPER FUNCTIONS

bool iterate(const QModelIndex &index, const QAbstractItemModel *model,
             std::function<bool(const QModelIndex&)> fun)
{
     if (index.isValid() && !fun(index))
         return false;

     if (!model->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
          return true;
     for (int i = 0; i < model->rowCount(index); ++i)
         if (!iterate(model->index(i, 0, index), model, fun))
             return false;
     return true;
}

CompleterProxyModel::CompleterProxyModel(QObject *parent)
    : QAbstractProxyModel(parent)
{
}

QModelIndex CompleterProxyModel::mapToSource(const QModelIndex &proxy) const
{
    int row = 0;
    QModelIndex foundIndex = {};
    iterate({}, sourceModel(), [&](const QModelIndex &ind) {
        QString str = ind.data().toString();
        if (!static_cast<DatabaseItem*>(ind.internalPointer())->isDir())
        {
            if (proxy.row() == row)
            {
                foundIndex = ind;
                return false;
            }
            ++row;
        }
        return true;
    });
    return foundIndex;
}

QModelIndex CompleterProxyModel::mapFromSource(const QModelIndex &source) const
{
    int row = 0;
    void *ptr = nullptr;
    iterate({}, sourceModel(), [&](const QModelIndex &ind) {
        if (ind == source)
        {
            ptr = ind.internalPointer();
            return false;
        }
        if (!static_cast<DatabaseItem*>(ind.internalPointer())->isDir())
            ++row;
        return true;
    });
    return createIndex(row, 0, ptr);
}

// BORING PART

QModelIndex CompleterProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    // Find the row-th item in the source model
    void *ptr = nullptr;
    int r = 0;

    iterate({}, sourceModel(), [&](const QModelIndex &ind) {
        if (!static_cast<DatabaseItem*>(ind.internalPointer())->isDir())
        {
            if (r == row)
            {
                ptr = ind.internalPointer();
                return false;
            }
            ++r;
        }
        return true;
    });
    return createIndex(row, column, ptr);
}

QModelIndex CompleterProxyModel::parent(const QModelIndex &index) const
{
    return {};
}

int CompleterProxyModel::rowCount(const QModelIndex &parent) const
{
    int count = 0;
    iterate(parent, sourceModel(), [&](const QModelIndex &ind) {
        if (!static_cast<DatabaseItem*>(ind.internalPointer())->isDir())
            ++count;
        return true;
    });
    return count;
}

int CompleterProxyModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CompleterProxyModel::data(const QModelIndex &index, int role) const
{
    QVariant data = sourceModel()->data(mapToSource(index), role);
    if (role != Qt::DisplayRole)
        return data;
    // Underlying object is of type header
    if (dynamic_cast<Header*>(static_cast<DatabaseItem*>(index.internalPointer())->getObject()))
        data = data.toString() + " [change]";
    return data;
}
