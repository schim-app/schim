#include "linearobjectarray.h"

LinearObjectArray::LinearObjectArray(Object *base, float deltaX, float deltaY, int count)
    : ObjectArray(base), deltaX(deltaX), deltaY(deltaY), count(count)
{
}

LinearObjectArray::LinearObjectArray(const LinearObjectArray &obj)
    : ObjectArray(obj)
{
    count = obj.count;
    deltaX = obj.deltaX;
    deltaY = obj.deltaY;
}

int LinearObjectArray::getCount()
{
    return count;
}

void LinearObjectArray::generate()
{
    qDeleteAll(getConstituents());
    getConstituents().clear();

    if (count == 0)
        return;
    getConstituents().reserve(count);

    // TODO preliminary remove baseObj->setParent(this);
    for (int i = 0; i < count; ++i)
    {
        Object *obj = baseObj->clone();
        obj->setPos(baseObj->getPos() + i * QPointF{deltaX, deltaY});
        // TODO preliminary remove obj->setParent(this);
        add(obj);
    }
}

Object *LinearObjectArray::clone() const
{
    return new LinearObjectArray(*this);
}
