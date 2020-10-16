#include "compositeobject.h"

CompositeObject::CompositeObject()
    : Object()
{

}

CompositeObject::~CompositeObject()
{
    for (Object *child : *this)
        delete child;
}
