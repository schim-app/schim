#include "glinearobjectarray.h"

GLinearObjectArray::GLinearObjectArray(LinearObjectArray *obj)
    : GCompositeObject(obj)
{
    if (get()->getConstituents().empty())
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

void GLinearObjectArray::reloadFromModel()
{
    if (get()->getConstituents().empty())
        get()->generate();
    GCompositeObject::reloadFromModel();
}

void GLinearObjectArray::applyToModel()
{
    // TODO implement
}
