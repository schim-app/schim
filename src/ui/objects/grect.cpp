#include "grect.h"

#include <QPainter>
#include <QRegion>
#include <QPointF>

GRect::GRect(Rect *obj)
    : GObject(obj) { }

QRectF GRect::boundingRect() const
{
    float infl = get()->linewidth;
    return get()->marginsAdded({infl, infl, infl, infl}).translated(-get()->getPos());
}

void GRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);

    auto pen = painter->pen();
    pen.setWidthF(get()->linewidth / 2);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);

    painter->drawRect(get()->translated(-get()->getPos()));
}

QPainterPath GRect::shape() const
{
    float infl = get()->linewidth;
    QRectF baseRect = get()->translated(-get()->getPos());
    // For some reason when I use margins{Added, Removed} with the same
    // argument, the hover area is not the same inside and outside the rectangle.
    QRectF outer = baseRect.marginsAdded({infl, infl, infl, infl}),
            inner = baseRect.marginsRemoved({infl * 2, infl * 2, infl * 2, infl * 2});

    QPainterPath path;
    path.addRect(outer);
    path.addRect(inner);

    path.closeSubpath();

    return path;
}

QVariant GRect::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
        get()->setPos(value.toPointF());
    return GObject::itemChange(change, value);
}

void GRect::reload()
{
    setPos(get()->getPos());
    if (handles != nullptr)
    {
        (*handles)[0]->setPos(0, 0);
        (*handles)[1]->setPos(get()->width(), 0);
        (*handles)[2]->setPos(get()->width(), get()->height());
        (*handles)[3]->setPos(0, get()->height());
    }
}

void GRect::apply()
{
    //TODO implement
}

void GRect::showHandles(bool show)
{
    if (show && handles == nullptr)
    {
        handles = new QList<GObjectHandle*> {
                new GObjectHandle(this),
                new GObjectHandle(this),
                new GObjectHandle(this),
                new GObjectHandle(this)
        };
        reload();
    }
    else
        return GObject::showHandles(show);
}

void GRect::handleChanged(GObjectHandle *handle)
{
    switch (handles->indexOf(handle))
    {
    case 0:
        get()->setTopLeft(mapToScene(handle->pos()));
        break;
    case 1:
        get()->setTopRight(mapToScene(handle->pos()));
        break;
    case 2:
        get()->setBottomRight(mapToScene(handle->pos()));
        break;
    case 3:
        get()->setBottomLeft(mapToScene(handle->pos()));
        break;
    }
    reload();
    if (scene())
        scene()->update();
}

Rect *GRect::get()
{
    return (Rect*) obj;
}

const Rect *GRect::get() const
{
    return (Rect*) obj;
}
