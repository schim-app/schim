#include "grect.h"

#include <QPainter>
#include <QRegion>

GRect::GRect(Rect *obj)
    : GObject(obj) { }

QRectF GRect::boundingRect() const
{
    float infl = get()->linewidth;
    return get()->marginsAdded({infl, infl, infl, infl});
}

void GRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);

    auto pen = painter->pen();
    pen.setWidthF(get()->linewidth / 2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->drawRect(*get());
}

QPainterPath GRect::shape() const
{
    float infl = get()->linewidth;
    QRectF outer = get()->marginsAdded({infl, infl, infl, infl}),
            inner = get()->marginsRemoved({infl, infl, infl, infl});

    QPainterPath path;
    path.addRect(outer);
    path.addRect(inner);

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
