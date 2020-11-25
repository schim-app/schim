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
    : list(list), scene(scene)
{
    parentList.reserve(list.size());
    for (auto *item : list)
        parentList.append(static_cast<GObject*>(item)->parentItem());
}

CmdDeleteSelection::~CmdDeleteSelection()
{
    if (isObsolete())
        for (auto *item : list)
            delete item;
}

void CmdDeleteSelection::undo()
{
    // TODO fix when some items are in a parent-child relation
    for (int i = 0; i < list.size(); ++i)
    {
        list[i]->setParentItem(parentList[i]);
        auto *modelObject = static_cast<GObject*>(list[i])->get();
        if (parentList[i] == nullptr)
            scene->getSheet()->append(modelObject);
        else
            parentList[i]->get()->append(modelObject);
        scene->addItem(list[i]);
    }
}

void CmdDeleteSelection::redo()
{
    for (auto *item : list)
    {
        auto *modelObject = static_cast<GObject*>(item)->get();
        if (item->parentItem())
            static_cast<GObject*>(item)->parentItem()->get()->removeAll(modelObject);
        else
            scene->getSheet()->removeAll(modelObject);
        scene->removeItem(item);
    }
}
