#include "linearobjectarray.h"

LinearObjectArray::LinearObjectArray(Object *base, float deltaX, float deltaY, int count)
    : ObjectArray(base), deltaX(deltaX), deltaY(deltaY), count(count)
{
}

int LinearObjectArray::getCount()
{
    return count;
}

Object *LinearObjectArray::generate(int index) const
{
    if (index < 0 || index >= count) // Out of bounds
        return nullptr;

    Object *obj = baseObj->clone();
    obj->setPos(baseObj->getPos() + index * QPointF{deltaX, deltaY});
    return obj;
}
