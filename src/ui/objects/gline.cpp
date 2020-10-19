#include "gline.h"

#include "ui/sheetscene.h"

#include <QtMath>
#include <QPainter>
//TODO temp
#include <QGraphicsSceneMouseEvent>
#include <iostream>
#include "ui/mainwindow.h"

GLine::GLine(Line *obj)
    : GObject(obj)
{
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
    // TODO improve this later; Namely, the handle size
    // should be invariant to the scene transformations
    float radius = get()->linewidth;
    float angle = get()->angle() * M_PI / 180, sin = -qSin(angle), cos = qCos(angle);
    QPointF ortho = {-radius * sin, radius * cos},
            coll = {radius * cos, radius * sin};

    QPolygonF poly;
    poly << -ortho - coll
         << ortho - coll
         << get()->p2() - get()->p1() + ortho + coll
         << get()->p2() - get()->p1() - ortho + coll;

    QPainterPath path;
    path.addPolygon(poly);
    path.closeSubpath();
    return path;
}

void GLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);
    auto pen = painter->pen();
    // We are dividing line width by 2 to compensate for antialiasing
    pen.setWidthF(get()->linewidth / 2);
    painter->setPen(pen);
    painter->drawLine(0, 0, get()->dx(), get()->dy());
}

QRectF GLine::boundingRect() const
{
    // TODO maybe make this more efficient
    return shape().boundingRect();
}

QVariant GLine::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
    {
        QPointF delta = get()->p2() - get()->p1();
        get()->setP1(value.toPointF());
        get()->setP2(value.toPointF() + delta);
    }
    return GObject::itemChange(change, value);
}

void GLine::reload()
{
    setPos(get()->p1());
    if (handles != nullptr)
    {
        (*handles)[0]->setPos(QPointF{0, 0});
        (*handles)[1]->setPos({get()->dx(), get()->dy()});
    }
}

void GLine::apply()
{
    get()->setP1((*handles)[0]->pos());
    get()->setP2((*handles)[1]->pos());
}

void GLine::handleChanged(GObjectHandle *handle)
{
    if (handle == (*handles)[0])
        get()->setP1(mapToScene(handle->pos()));
    else if (handle == (*handles)[1])
        get()->setP2(mapToScene(handle->pos()));
    reload();
    //TODO Is there any way I can do this without updating the whole scene?
    // Currently, if I don't do this there are graphical artifacts
    if (scene())
        scene()->update();
}

void GLine::showHandles(bool show)
{
    if (show && handles == nullptr)
    {
        handles = new QList<GObjectHandle*> {
                new GObjectHandle(this), // point 1
                new GObjectHandle(this) // point 2
        };
        reload();
    }
    else
        GObject::showHandles(show);
}
