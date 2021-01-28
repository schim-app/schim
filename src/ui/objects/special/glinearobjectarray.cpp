#include "glinearobjectarray.h"

GLinearObjectArray::GLinearObjectArray(LinearObjectArray *obj)
    : GCompositeObject(obj)
{
    if (get()->empty())
        get()->generate();
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
    if (get()->empty())
        get()->generate();
    GCompositeObject::reload();
}

void GLinearObjectArray::apply()
{
    // TODO implement
}
