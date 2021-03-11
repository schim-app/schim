#include "commands.h"

#include <algorithm>

CmdInsertObject::CmdInsertObject(GObject *obj, SheetScene *scene, GCompositeObject *parent)
    : obj(obj), parent(parent), scene(scene) { }

CmdInsertObject::~CmdInsertObject()
{
    if (isObsolete())
        delete obj;
}

void CmdInsertObject::undo()
{
    scene->removeItem(obj);
    scene->getSheet()->getObjects().removeAll(obj->get());
}

void CmdInsertObject::redo()
{
    if (parent)
    {
        obj->setParentItem(parent);
        parent->get()->add(obj->get());
    }
    else
    {
        scene->addItem(obj);
        scene->getSheet()->addObject(obj->get());
    }
}

//////////////////////////////////////////////////////////////////////////

CmdDeleteSelection::CmdDeleteSelection(QList<QGraphicsItem *> list, SheetScene *scene)
    : list(list), scene(scene)
{
    parentList.reserve(list.size());
    foreach (auto *item, list)
        parentList.append(static_cast<GObject*>(item)->parentItem());
}

CmdDeleteSelection::~CmdDeleteSelection()
{
    if (isObsolete())
        foreach (auto *item, list)
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
            scene->getSheet()->addObject(modelObject);
        else
            parentList[i]->get()->add(modelObject);
        scene->addItem(list[i]);
    }
}

void CmdDeleteSelection::redo()
{
    foreach (auto *item, list)
    {
        auto *modelObject = static_cast<GObject*>(item)->get();
        if (item->parentItem())
            static_cast<GObject*>(item)->parentItem()->get()->remove(modelObject);
        else
            scene->getSheet()->getObjects().removeAll(modelObject);
        scene->removeItem(item);
    }
}

//////////////////////////////////////////////////////////////////////////

CmdChangeHeader::CmdChangeHeader(Header *newHeader, SheetScene *scene)
    : newHeader(newHeader), oldHeader(scene->getSheet()->getHeader()), scene(scene)
{
}

CmdChangeHeader::~CmdChangeHeader()
{
    if (isObsolete())
    {
        // Delete the header that is not currently in the sheet
        if (scene->getSheet()->getHeader() == newHeader)
            delete oldHeader;
        else
            delete newHeader;
    }
}

void CmdChangeHeader::undo()
{
    scene->setHeader(oldHeader, false); // newHeader will be automatically deleted
}

void CmdChangeHeader::redo()
{
    scene->setHeader(newHeader, false);
}
