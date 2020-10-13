#include "gheader.h"
#include "gline.h"
#include "gobject.h"
#include "grect.h"
#include "global.h"

#include <model/header.h>
#include <model/rect.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

#define if_cast(TYPE, object) if (dynamic_cast<TYPE>(object))

GObject::GObject(Object *obj)
    : obj(obj)
{
    setFlags(
                QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable
                );
    setAcceptHoverEvents(true);
}

const Object *GObject::get() const
{
    return obj;
}

Object *GObject::get()
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
    auto pen = painter->pen();

    // The order is important
    if (option->state & QStyle::State_MouseOver)
        pen.setColor(colorHover);

    if (option->state & QStyle::State_Selected)
        pen.setColor(colorSelected);

    painter->setPen(pen);
    return;
}

void GObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    update(boundingRect());
    QGraphicsItem::hoverEnterEvent(event);
}
