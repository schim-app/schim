#include "grect.h"

#include <QPainter>
#include <QPointF>

GRect::GRect(Rect *obj)
    : GObject(obj) { }

// GETTERS

Rect *GRect::get()
{
    return (Rect*) obj;
}

const Rect *GRect::get() const
{
    return (Rect*) obj;
}

// OVERRIDDE QGraphicsItem

QRectF GRect::boundingRect() const
{
    // TODO Why is get() sometimes null here??
    float infl = get()->getLinewidth(); // Inflation
    if (get())
    {
        auto x = get()->marginsAdded({infl, infl, infl, infl});
        auto y = get()->getPos();
        return x.translated(-y);
    }
    else
        return {};
}

void GRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);

    auto pen = painter->pen();
    pen.setWidthF(get()->getLinewidth() / 2);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setCosmetic(cosmetic);
    if (cosmetic)
        pen.setWidthF(1.5);
    painter->setPen(pen);

    painter->drawRect(get()->translated(-get()->getPos()));
}

QPainterPath GRect::shape() const
{
    float infl = get()->getLinewidth();
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

// OBJECT EDITING

void GRect::reloadFromModel()
{
    setPos(get()->getPos());
    if (handles != nullptr)
    {
        for (auto *handle : *handles)
            handle->setFlag(ItemSendsGeometryChanges, false);

        (*handles)[0]->setPos(0, 0);
        (*handles)[1]->setPos(get()->width(), 0);
        (*handles)[2]->setPos(get()->width(), get()->height());
        (*handles)[3]->setPos(0, get()->height());

        for (auto *handle : *handles)
            handle->setFlag(ItemSendsGeometryChanges, true);
    }
}

void GRect::applyToModel()
{
    get()->setPos(pos());
}

void GRect::showHandles(bool show)
{
    if (show && handles == nullptr)
    {
        // There are 4 handles - in the corners of the rectangle
        handles = new QList<GObjectHandle*> {
                new GObjectHandle(this),
                new GObjectHandle(this),
                new GObjectHandle(this),
                new GObjectHandle(this)
        };
        // Place the handles in their proper location
        reloadFromModel();
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
    // Fix negative width/height issue
    if (get()->top() > get()->bottom())
    {
        // Swap top and bottom so that top < bottom
        auto top = get()->top();
        get()->setTop(get()->bottom());
        get()->setBottom(top);

        // Flip handles around horizontal axis
        std::swap((*handles)[0], (*handles)[3]);
        std::swap((*handles)[1], (*handles)[2]);
    }
    if (get()->left() > get()->right())
    {
        // Swap left and right so that left < right
        auto left = get()->left();
        get()->setLeft(get()->right());
        get()->setRight(left);

        // Flip handles around vertical axis
        std::swap((*handles)[0], (*handles)[1]);
        std::swap((*handles)[2], (*handles)[3]);
    }
    reloadFromModel();
    if (scene())
        scene()->update();
}
