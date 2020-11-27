#include "objectarray.h"

ObjectArray::ObjectArray(Object *base)
    : baseObj(base) { }

ObjectArray::~ObjectArray() { }

void ObjectArray::rebase(int index)
{
    //TODO implement
}

int ObjectArray::getCount()
{
    return 0; //TODO
}

Object *ObjectArray::getBaseObject()
{
    return baseObj;
}
