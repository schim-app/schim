#include "gheader.h"
#include "gline.h"
#include "gobject.h"
#include "grect.h"
#include "global.h"

#include <model/header.h>
#include <model/rect.h>

#include <QGraphicsSceneMouseEvent>

#define if_cast_return(TYPE, object) if (dynamic_cast<TYPE*>(object)) return new G##TYPE((TYPE*)object)

/***********
 * GObject *
************/

GObject::GObject(Object *obj)
    : obj(obj)
{
    setFlags(
                QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable |
                ItemSendsGeometryChanges
                );
    setAcceptHoverEvents(true);
    setPos(obj->getPos());
    setRotation(0);
}

GObject::~GObject()
{
    if (handles != nullptr)
    {
        for (auto *handle : *handles)
            delete handle;
        delete handles;
    }
}

const Object *GObject::get() const
{
    return obj;
}

Object *GObject::get()
{
    return obj;
}

bool GObject::isHovered() const
{
    return hovered;
}

GObject *GObject::parentItem()
{
    return (GObject *) QGraphicsItem::parentItem();
}

GObject *GObject::assign(Object *obj)
{
    if_cast_return(Line, obj);
    if_cast_return(Rect, obj);
    if_cast_return(Header, obj);
    if_cast_return(CompositeObject, obj);
    return nullptr;
}

QRectF GObject::boundingRect() const
{
    return childrenBoundingRect();
}

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    auto pen = painter->pen();

    // The order is important
    if (isHovered() || (parentItem() &&  parentItem()->isHovered()))
        pen.setColor(colorHover);

    if (isSelected() || (parentItem() && parentItem()->isSelected()))
        pen.setColor(colorSelected);

    painter->setPen(pen);
}

void GObject::reload() { }

void GObject::apply() { }

void GObject::showHandles(bool show)
{
    if (show && handles == nullptr)
        moveHandlesAbove();
    if (!show && handles != nullptr)
    {
        for (auto *handle : *handles)
            delete handle;
        delete handles;
        handles = nullptr;
    }
}

void GObject::handleChanged(GObjectHandle *) { }

void GObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    if (flags() & ItemIsSelectable)
        hovered = true;
}

void GObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    if (flags() & ItemIsSelectable)
        hovered = false;
}

QVariant GObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemZValueHasChanged)
        moveHandlesAbove();
    else if (change == ItemSelectedHasChanged)
        showHandles(value.toBool());
    else if (change == ItemSceneHasChanged && value.isNull())
    {
        setSelected(false);
        hovered = false;
    }

    return QGraphicsItem::itemChange(change, value);
}

void GObject::moveHandlesAbove()
{
    if (handles != nullptr)
        for (auto *handle : *handles)
            handle->setZValue(zValue() + 1);
}

/*****************
 * GObjectHandle *
******************/

GObjectHandle::GObjectHandle(GObject *obj)
    : QGraphicsRectItem(-1, -1, 2, 2),
      obj(obj)
{
    setAcceptHoverEvents(false);
    setFlags(ItemSendsGeometryChanges);

    setParentItem(obj);

    auto _pen = pen();
    _pen.setJoinStyle(Qt::MiterJoin);
    _pen.setColor(colorSelected);
    setPen(_pen);
    setBrush(colorSelected);

}

QVariant GObjectHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged)
        obj->handleChanged(this);

    return QGraphicsRectItem::itemChange(change, value);
}

void GObjectHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        _dragStartPos = event->scenePos();
        event->accept();
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}

void GObjectHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto delta = event->scenePos() - _dragStartPos;
        moveBy(delta.x(), delta.y()); //TODO 1
        _dragStartPos = event->scenePos();

        event->accept();
    }
    else
        QGraphicsRectItem::mouseMoveEvent(event);
}
