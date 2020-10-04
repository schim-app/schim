#include "gline.h"
#include "gobject.h"

#define if_cast(TYPE, object) if (dynamic_cast<TYPE>(object))

GObject::GObject(Object *obj)
    : obj(obj) { }

const Object *GObject::getObject() const
{
    return obj;
}

Object *GObject::getObject()
{
    return obj;
}

GObject *GObject::createObject(Object *obj)
{
    if_cast(Line*, obj) return new GLine((Line*) obj);
}
