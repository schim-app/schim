#include "commands.h"

#include <algorithm>

CmdInsertLine::CmdInsertLine(GLine *line, SheetScene *sheet)
    : line(line), sheet(sheet) { }

void CmdInsertLine::undo()
{
    sheet->removeItem(line);
    sheet->getSheet()->removeAll(line->get());
}

void CmdInsertLine::redo()
{
    sheet->addItem(line);
    sheet->getSheet()->append(line->get());
}
