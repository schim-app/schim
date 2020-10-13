#include "grect.h"

#include <QPainter>
#include <QRegion>

GRect::GRect(Rect *obj)
    : GObject(obj) { }

QRectF GRect::boundingRect() const
{
    return shape().boundingRect();
}

void GRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);

    auto pen = painter->pen();
    pen.setWidthF(get()->linewidth);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawRect(*get());
}

QPainterPath GRect::shape() const
{
    float infl = 2 * get()->linewidth;
    QRectF outer = get()->marginsAdded({infl, infl, infl, infl}),
            inner = get()->marginsRemoved({1.5 * infl, 1.5 * infl, 1.5 *infl, 1.5 * infl});

    QPainterPath path;
    path.addRegion(QRegion(outer.toRect()).subtracted(inner.toRect()));

    return path;
}

Rect *GRect::get()
{
    return (Rect*) obj;
}

const Rect *GRect::get() const
{
    return (Rect*) obj;
}
