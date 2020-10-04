#include "object.h"

Object::Object()
{

}

Object::~Object()
{
    for (Object *child : children)
        delete child;
}

void Object::addChild(Object *child)
{
    children.append(child);
}
