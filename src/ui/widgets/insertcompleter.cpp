#include "insertcompleter.h"
#include "ui_insertcompleter.h"

#include "fileio/database.h"
#include "global.h"

#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>

InsertCompleter::InsertCompleter(QWidget *parent) :
    QDialog(parent, Qt::SplashScreen),
    ui(new Ui::InsertCompleter)
{
    ui->setupUi(this);

    // Create and configure the completer
    completer = new Completer(globalDatabase, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionRole(Qt::DisplayRole);
    completer->setFilterMode(Qt::MatchContains);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    ui->edit->setCompleter(completer);

    setFocusProxy(ui->edit);

    connect(completer,static_cast<void (QCompleter::*)(const QModelIndex&)>(&QCompleter::activated),
            this, &InsertCompleter::onActivated);
}

InsertCompleter::~InsertCompleter()
{
    delete ui;
}

void InsertCompleter::setVisible(bool visible)
{
    QDialog::setVisible(visible);
    if (visible)
    {
        ui->edit->clear();
        connect(qApp, &QApplication::focusChanged, this, &InsertCompleter::onFocusChanged);
    }
    else
        disconnect(qApp, &QApplication::focusChanged, this, &InsertCompleter::onFocusChanged);
    // TODO Show popup right at the start.
    // This should be trivial with completer->complete()
    // but for some reason once you type in the first character
    // the popup disappears.
    // completer->complete();
}

void InsertCompleter::onActivated(const QModelIndex &index)
{
    // Start an insert operation for the selected object type
    auto *completionModel = static_cast<QAbstractProxyModel*>(completer->completionModel());
    QModelIndex ind = completionModel->mapToSource(index);
    auto *databaseItem = static_cast<DatabaseItem*>(ind.internalPointer());
    hide();
    emit insertionRequested(*databaseItem->getObject());
}

void InsertCompleter::onFocusChanged(QWidget *a, QWidget *b)
{
    if (a == ui->edit && b != completer->popup())
        hide();
}

// EVENTS

void InsertCompleter::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        hide();
}

void InsertCompleter::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        _moveOffset = event->globalPos() - frameGeometry().topLeft();
    QDialog::mousePressEvent(event);
}

void InsertCompleter::mouseMoveEvent(QMouseEvent *event)
{
    // TODO movement is not very responsive, although it is smooth.
    // Could be a problem with my tiling window manager?
    if (event->buttons() == Qt::LeftButton)
        move(event->globalPos() - _moveOffset);
}
