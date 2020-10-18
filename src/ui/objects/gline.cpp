#include "gline.h"

#include "ui/sheetscene.h"

#include <QtMath>
#include <QPainter>
//TODO temp
#include <iostream>


GLine::GLine(Line *obj)
    : GObject(obj)
{
    handles = {
        new GObjectHandle(this),
        new GObjectHandle(this),
    };
    setPos({});
    reload();
}

Line *GLine::get()
{
    return (Line*) obj;
}

const Line *GLine::get() const
{
    return (Line*) obj;
}

QPainterPath GLine::shape() const
{
    // TODO what about a null line?
    QPolygonF poly;
    qreal selectionOffset = get()->linewidth;
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
    pen.setWidthF(get()->linewidth / 2);
    painter->setPen(pen);
    painter->drawLine(get()->x1(), get()->y1(), get()->x2(), get()->y2());
}

void GLine::reload()
{
    handles[0]->setCenterPos(mapFromScene(get()->p1()));
    handles[1]->setCenterPos(mapFromScene(get()->p2()));
}

void GLine::apply()
{
    get()->setP1(handles[0]->pos());
    get()->setP2(handles[1]->pos());
}

//TODO temp test
void GLine::update(const QRectF &rect)
{
    std::cout << "update\n";
    GObject::update();
}

void GLine::handleChanged(GObjectHandle *handle)
{
    if (handle == handles[0])
        get()->setP1(handle->getCenterPos());
    if (handle == handles[1])
        get()->setP2(handle->getCenterPos());
    std::cout << "handleChanged\n";
}

