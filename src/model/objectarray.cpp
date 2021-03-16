#include "objectarray.h"

ObjectArray::ObjectArray(Object *base)
    : baseObj(base) { }

ObjectArray::ObjectArray(const ObjectArray &obj)
{
    variables = obj.variables;
    delete baseObj;
    baseObj = obj.baseObj->clone();
}

ObjectArray::~ObjectArray()
{
    delete baseObj;
}

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
