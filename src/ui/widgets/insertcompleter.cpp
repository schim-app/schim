#include "insertcompleter.h"
#include "ui_insertcompleter.h"

#include "fileio/database.h"
#include "global.h"

#include <QEvent>
#include <QKeyEvent>

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

    connect(completer, static_cast<void (QCompleter::*)(const QModelIndex&)>(&QCompleter::activated), this, &InsertCompleter::onActivated);
}

InsertCompleter::~InsertCompleter()
{
    delete ui;
}

void InsertCompleter::setFocus(Qt::FocusReason reason)
{
    ui->edit->clear();
    ui->edit->setFocus(reason);
}

void InsertCompleter::onActivated(const QModelIndex &index)
{
    auto *completionModel = static_cast<QAbstractProxyModel*>(completer->completionModel());
    QModelIndex ind = completionModel->mapToSource(index);
    auto *databaseItem = static_cast<DatabaseItem*>(ind.internalPointer());
    hide();
    emit insertionRequested(databaseItem->getObject());
}

// EVENTS

void InsertCompleter::focusOutEvent(QFocusEvent *event)
{
    // TODO rm maybe
    Q_UNUSED(event)
}

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
