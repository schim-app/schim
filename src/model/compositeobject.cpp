#include "compositeobject.h"

CompositeObject::CompositeObject()
    : Object()
{

}

CompositeObject::~CompositeObject()
{
    for (Object *child : children)
        delete child;
}

void CompositeObject::addChild(Object *child)
{
    children.append(child);
}
