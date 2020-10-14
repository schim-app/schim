#include "gheader.h"
#include "gline.h"
#include "gobject.h"
#include "grect.h"
#include "global.h"

#include <model/header.h>
#include <model/rect.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#define if_cast_return(TYPE, object) if (dynamic_cast<TYPE*>(object)) return new G##TYPE((TYPE*)object)

GObject::GObject(Object *obj)
    : obj(obj)
{
    setFlags(
                QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable
                );
    setAcceptHoverEvents(true);
    setPos(obj->getPos());
}

const Object *GObject::get() const
{
    return obj;
}

Object *GObject::get()
{
    return obj;
}

bool GObject::isHovered() const
{
    return hovered;
}

GObject *GObject::parentItem()
{
    return (GObject *) QGraphicsItem::parentItem();
}

GObject *GObject::assign(Object *obj)
{
    if_cast_return(Line, obj);
    if_cast_return(Rect, obj);
    if_cast_return(Header, obj);
    if_cast_return(CompositeObject, obj);
    return nullptr;
}

QRectF GObject::boundingRect() const
{
    return {};
}

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto pen = painter->pen();

    // The order is important
    if (isHovered() || (parentItem() &&  parentItem()->isHovered()))
        pen.setColor(colorHover);

    if (isSelected() || (parentItem() && parentItem()->isSelected()))
        pen.setColor(colorSelected);

    painter->setPen(pen);
}

void GObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (dynamic_cast<GHeader*>(this)) return;
    QGraphicsItem::hoverEnterEvent(event);
    update(boundingRect());
    if (flags() & ItemIsSelectable)
        hovered = true;
}

void GObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (dynamic_cast<GHeader*>(this)) return;
    QGraphicsItem::hoverLeaveEvent(event);
    if (flags() & ItemIsSelectable)
        hovered = false;
}
