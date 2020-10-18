#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <model/sheet.h>
#include "model/line.h"
#include "sheetscene.h"
#include "ui/objects/gline.h"

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

#endif // COMMANDS_H
