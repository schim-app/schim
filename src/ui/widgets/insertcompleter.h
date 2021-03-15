#ifndef INSERTCOMPLETER_H
#define INSERTCOMPLETER_H

#include "completer.h"
#include "model/object.h"
#include "ui/vim.h"

#include <QDialog>

namespace Ui {
class InsertCompleter;
}

class InsertCompleter : public QDialog
{
    Q_OBJECT

public:
    explicit InsertCompleter(QWidget *parent);
    ~InsertCompleter();

    void setVisible(bool visible) override;

signals:
    /**
     * @brief Emitted when the user has selected an item for insertion
     * into the scene.
     * @param obj The object to place into the scene.
     *
     * @note Do not place `obj` into the scene directly -- clone it first.
     */
    void insertionRequested(const Object &obj);

private slots:
    /**
     * @brief Called when an entry from the completer has been chosen.
     * @param index Index of the selected entry in the `completionModel()`.
     */
    void onActivated(const QModelIndex &index);
    void onFocusChanged(QWidget *a, QWidget *b);

private:
    // EVENTS
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    // ATTRIBUTES
    Ui::InsertCompleter *ui;
    QPoint _moveOffset; // Used when moving the completer with the mouse
    Completer *completer{};
};

#endif // INSERTCOMPLETER_H
