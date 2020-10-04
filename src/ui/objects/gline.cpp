#include "gline.h"

GLine::GLine(Line *obj)
    : GObject(obj) { }

QRectF GLine::boundingRect() const
{
    return QGraphicsLineItem::boundingRect();
}

void GLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsLineItem::paint(painter, option, widget);
}
