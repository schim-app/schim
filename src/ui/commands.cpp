#include "commands.h"

#include <algorithm>

CmdInsertObject::CmdInsertObject(GObject *obj, SheetScene *sheet)
    : obj(obj), sheet(sheet) { }

void CmdInsertObject::undo()
{
    sheet->removeItem(obj);
    sheet->getSheet()->removeAll(obj->get());
}

void CmdInsertObject::redo()
{
    sheet->addItem(obj);
    sheet->getSheet()->append(obj->get());
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
    {
        sheet->addItem(item);
        sheet->getSheet()->append(((GObject*) item)->get());
    }
}

void CmdDeleteSelection::redo()
{
    for (auto *item : list)
    {
        sheet->removeItem(item);
        sheet->getSheet()->removeAll(((GObject*) item)->get());
    }
}
