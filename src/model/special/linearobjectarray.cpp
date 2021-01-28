#include "linearobjectarray.h"

LinearObjectArray::LinearObjectArray(Object *base, float deltaX, float deltaY, int count)
    : ObjectArray(base), deltaX(deltaX), deltaY(deltaY), count(count)
{
}

int LinearObjectArray::getCount()
{
    return count;
}

void LinearObjectArray::generate()
{
    qDeleteAll(*this);
    clear();

    if (count == 0)
        return;
    reserve(count);

    append(baseObj);
    baseObj->setParent(this);
    for (int i = 1; i < count; ++i)
    {
        Object *obj = baseObj->clone();
        obj->setPos(baseObj->getPos() + i * QPointF{deltaX, deltaY});
        obj->setParent(this);
        append(obj);
    }
}
