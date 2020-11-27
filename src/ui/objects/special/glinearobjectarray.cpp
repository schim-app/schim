#include "glinearobjectarray.h"

GLinearObjectArray::GLinearObjectArray(LinearObjectArray *obj)
    : GObject(obj)
{
}

GLinearObjectArray::~GLinearObjectArray()
{
    clear();
}

LinearObjectArray *GLinearObjectArray::get()
{
    return static_cast<LinearObjectArray*>(obj);
}

const LinearObjectArray *GLinearObjectArray::get() const
{
    return static_cast<LinearObjectArray*>(obj);
}

void GLinearObjectArray::reload()
{
    clear();
    for (int i = 0; i < get()->getCount(); ++i)
    {
        auto *item = GObject::assign(get()->generate(i));
        scene()->addItem(item);
        item->setParentItem(this);
        item->setFlags({});
    }
}

void GLinearObjectArray::apply()
{
    // TODO implement
}

void GLinearObjectArray::clear()
{
    for (auto *_child : childItems())
    {
        GObject *child = static_cast<GObject*>(_child);
        scene()->removeItem(child);
        delete child->get();
        delete child;
    }
}
