#include "commands.h"

#include <algorithm>

CmdInsertObject::CmdInsertObject(GObject *obj, SheetScene *scene)
    : obj(obj), scene(scene) { }

CmdInsertObject::~CmdInsertObject()
{
    if (isObsolete())
        delete obj;
}

void CmdInsertObject::undo()
{
    scene->removeItem(obj);
    scene->getSheet()->removeAll(obj->get());
}

void CmdInsertObject::redo()
{
    scene->addItem(obj);
    scene->getSheet()->append(obj->get());
}

//////////////////////////////////////////////////////////////////////////

CmdDeleteSelection::CmdDeleteSelection(QList<QGraphicsItem *> list, SheetScene *scene)
    : list(list), scene(scene) { }

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
        scene->addItem(item);
        scene->getSheet()->append(((GObject*) item)->get());
    }
}

void CmdDeleteSelection::redo()
{
    for (auto *item : list)
    {
        scene->removeItem(item);
        scene->getSheet()->removeAll(((GObject*) item)->get());
    }
}
