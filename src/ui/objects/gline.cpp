#include "gline.h"

#include <QPainter>
#include <QtMath>

GLine::GLine(Line *obj)
    : GObject(obj) { }

Line *GLine::get()
{
    return (Line*) obj;
}

const Line *GLine::get() const
{
    return (Line*) obj;
}

QRectF GLine::boundingRect() const
{
    return shape().boundingRect();
}

QPainterPath GLine::shape() const
{
    // TODO what about a null line?
    QPolygonF poly;
    qreal selectionOffset = 4 * get()->linewidth;
    qreal radAngle = get()->angle() * M_PI / 180;
    qreal dx = selectionOffset * sin(radAngle);
    qreal dy = selectionOffset * cos(radAngle);
    QPointF offset1 = QPointF(dx, dy);
    QPointF offset2 = QPointF(-dx, -dy);
    poly << get()->p1() + offset1
             << get()->p1() + offset2
             << get()->p2() + offset2
             << get()->p2() + offset1;

    QPainterPath path;
    path.addPolygon(poly);
    return path;
}

void GLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);
    auto pen = painter->pen();
    pen.setWidthF(get()->linewidth);
    painter->setPen(pen);
    painter->drawLine(get()->x1(), get()->y1(), get()->x2(), get()->y2());
}
