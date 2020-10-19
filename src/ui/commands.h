#ifndef COMMANDS_H
#define COMMANDS_H

#include <model/sheet.h>
#include "model/line.h"
#include "sheetscene.h"
#include "ui/objects/gline.h"

#include <QUndoCommand>

class CmdInsertLine : public QUndoCommand
{
public:
    CmdInsertLine(GLine *line, SheetScene *sheet);
    void undo() override;
    void redo() override;

private:
    GLine *line;
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
