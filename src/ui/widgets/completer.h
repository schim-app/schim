#ifndef COMPLETER_H
#define COMPLETER_H

#include "fileio/database.h"

#include <QCompleter>
#include <QSortFilterProxyModel>

class CompleterProxyModel : public QAbstractProxyModel
{

public:
    CompleterProxyModel(QObject *parent = nullptr);

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    // BORING PART
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Database *sourceModel() const;
};

class Completer : public QCompleter
{
    Q_OBJECT

public:
    explicit Completer(QObject *parent = nullptr);
    explicit Completer(Database *model, QObject *parent = nullptr);
    ~Completer();

    CompleterProxyModel *model();
};

#endif // COMPLETER_H
