#include "gheader.h"
#include "gline.h"
#include "gobject.h"
#include "grect.h"

#include <model/header.h>
#include <model/rect.h>

#define if_cast(TYPE, object) if (dynamic_cast<TYPE>(object))

GObject::GObject()
{
}

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

GObject *GObject::assign(Object *obj)
{
    if_cast(Line*, obj) return new GLine((Line*) obj);
    if_cast(Rect*, obj) return new GRect((Rect*) obj);
    if_cast(Header*, obj) return new GHeader((Header*) obj);
}

QRectF GObject::boundingRect() const
{
    return {};
}

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    return;
}
