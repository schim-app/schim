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

// GETTERS

Line *GLine::get()
{
    return (Line*) obj;
}

const Line *GLine::get() const
{
    return (Line*) obj;
}

// OVERRIDEN QGraphicsItem METHODS

QPainterPath GLine::shape() const
{
    // TODO BUG why is the item sometimes selectable even when the mouse is inside
    // shape (verified by drawing shape around the line)

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
    pen.setCosmetic(cosmetic);
    if (cosmetic)
        pen.setWidthF(1.5);
    painter->setPen(pen);
    painter->drawLine(0, 0, get()->dx(), get()->dy());

    return;
    //TODO temporary -- draw the shape for debugging
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawPath(shape());
}

QRectF GLine::boundingRect() const
{
    // TODO maybe make this more efficient
    return shape().boundingRect();
}

// FOR EDITING THE OBJECT

void GLine::reload()
{
    setPos(get()->p1());
    if (handles != nullptr)
    {
        for (auto *handle : *handles)
            handle->setFlag(ItemSendsGeometryChanges, false);

        (*handles)[0]->setPos(QPointF{0, 0});
        (*handles)[1]->setPos({get()->dx(), get()->dy()});

        for (auto *handle : *handles)
            handle->setFlag(ItemSendsGeometryChanges, true);
    }
}

void GLine::apply()
{
    if (handles)
    {
        get()->setP1((*handles)[0]->scenePos());
        get()->setP2((*handles)[1]->scenePos());
    }
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
