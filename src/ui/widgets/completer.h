/// @file completer.h
#ifndef COMPLETER_H
#define COMPLETER_H

#include "fileio/database.h"
#include "ui/vim.h"

#include <QCompleter>
#include <QLineEdit>
#include <QSortFilterProxyModel>

class Completer;

/**
 * @brief Derived line edit to use with the completer.
 *
 * @note This class exists only so that we can implement
 * vim bindings for the line edit used with `Completer`.
 * We would have simply installed an eventFilter to a normal
 * `QLineEdit`, but it doesn't receive events while the
 * completer popup is shown.
 */
class CompleterEdit : public QLineEdit
{
public:
    CompleterEdit(QWidget *parent = nullptr);

private:
    // EVENTS
    void keyPressEvent(QKeyEvent *event);
    bool processVimAction(const Vim::Action &action);
};

/**
 * @brief A proxy model that flattens a `Database` into a list model.
 */
class CompleterProxyModel : public QAbstractProxyModel
{

public:
    CompleterProxyModel(QObject *parent = nullptr);

    /**
     * @brief Given an index in the proxy, obtain an index in the source.
     */
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    /**
     * @brief Given an index in the source, obtain an index in the model.
     */
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

    // BORING PART
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Database *sourceModel() const;
};

/**
 * @brief Custom `QCompleter`.
 *
 * Currently only creates (and deletes) its own `CompleterProxyModel` for a
 * `Database` model.
 */
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
