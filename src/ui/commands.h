#ifndef COMMANDS_H
#define COMMANDS_H

#include <model/sheet.h>
#include "sheetscene.h"
#include "ui/objects/gobject.h"

#include <QUndoCommand>

class CmdInsertObject : public QUndoCommand
{
public:
    CmdInsertObject(GObject *obj, SheetScene *sheet);
    void undo() override;
    void redo() override;

private:
    GObject *obj;
    SheetScene *sheet;
};

class CmdDeleteSelection : public QUndoCommand
{
public:
    CmdDeleteSelection(QList<QGraphicsItem *> list, SheetScene *sheet);
    ~CmdDeleteSelection();
    void undo() override;
    void redo() override;

private:
    QList<QGraphicsItem *> list;
    SheetScene *sheet;
};

#endif // COMMANDS_H
