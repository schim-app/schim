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

//////////////////////////////////////////////////////////////////////////

CmdDeleteSelection::CmdDeleteSelection(QList<QGraphicsItem *> list, SheetScene *sheet)
    : list(list), sheet(sheet) { }

CmdDeleteSelection::~CmdDeleteSelection()
{
    if (isObsolete())
        for (auto *item : list)
            delete item;
}

void CmdDeleteSelection::undo()
{
    for (auto *item : list)
        sheet->addItem(item);
}

void CmdDeleteSelection::redo()
{
    for (auto *item : list)
        sheet->removeItem(item);
}
