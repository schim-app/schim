#include "grect.h"

GRect::GRect(Rect *obj)
    : GObject(obj), QGraphicsRectItem(obj->pivot.x(), obj->pivot.y(), obj->width, obj->height)
{
}

QRectF GRect::boundingRect() const
{
    return QGraphicsRectItem::boundingRect();
}

void GRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
