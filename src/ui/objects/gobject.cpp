#include "gheader.h"
#include "gline.h"
#include "gobject.h"
#include "grect.h"
#include "global.h"

#include <model/header.h>
#include <model/rect.h>

#define if_cast_return(TYPE, object) if (dynamic_cast<TYPE*>(object)) return new G##TYPE((TYPE*)object)

/***********
 * GObject *
************/

GObject::GObject(Object *obj)
    : obj(obj)
{
    setFlags(
                QGraphicsItem::ItemIsMovable |
                QGraphicsItem::ItemIsSelectable
                );
    setAcceptHoverEvents(true);
    setPos(obj->getPos());
}

GObject::~GObject()
{
    for (auto *handle : handles)
        delete handle;
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

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto pen = painter->pen();

    // The order is important
    if (isHovered() || (parentItem() &&  parentItem()->isHovered()))
        pen.setColor(colorHover);

    if (_handleSelected || isSelected() || (parentItem() && parentItem()->isSelected()))
        pen.setColor(colorSelected);

    //TODO temp
    painter->drawEllipse(QRectF{-5, -5, 10, 10});

    painter->setPen(pen);
}

void GObject::reload() { }

void GObject::apply() { }

void GObject::showHandles(bool show)
{
    if (show)
        for (auto *handle : handles)
            handle->show();
    else
        for (auto *handle : handles)
            handle->hide();
}

void GObject::handleChanged(GObjectHandle *handle) { }

void GObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    update();
    if (flags() & ItemIsSelectable)
        hovered = true;
}

void GObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    if (flags() & ItemIsSelectable)
        hovered = false;
}

void GObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _objectAboutToBeSelected = true;
    QGraphicsItem::mousePressEvent(event);
}

QVariant GObject::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSceneHasChanged && scene() != nullptr)
        for (auto *handle : handles)
        {
            moveHandlesAbove();
            scene()->addItem(handle);
        }
    else if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        showHandles(_handleAboutToBeSelected || value.toBool());
        if (value == false && !_handleAboutToBeSelected)
            _handleSelected = false;
        _handleAboutToBeSelected = false;
    }
    else if (change == QGraphicsItem::ItemZValueHasChanged)
        moveHandlesAbove();

    return QGraphicsItem::itemChange(change, value);
}

void GObject::moveHandlesAbove()
{
    for (auto *handle : handles)
        handle->setZValue(zValue() + 1);
}

/*****************
 * GObjectHandle *
******************/

GObjectHandle::GObjectHandle(GObject *obj)
    : QGraphicsRectItem(-1, -1, 2, 2),
      obj(obj)
{
    hide();
    setAcceptHoverEvents(false);
    setFlags(ItemSendsGeometryChanges);

    setParentItem(obj);

    auto _pen = pen();
    _pen.setJoinStyle(Qt::MiterJoin);
    _pen.setColor(colorSelected);
    setPen(_pen);
    setBrush(colorSelected);

}

void GObjectHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}

QVariant GObjectHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange)
        obj->update();
    else if (change == QGraphicsItem::ItemPositionHasChanged)
        obj->handleChanged(this);
    else if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        if (value == false && !obj->_objectAboutToBeSelected)
        {
            obj->_handleSelected = false;
            obj->update();
            obj->showHandles(false);
        }
        obj->_objectAboutToBeSelected = false;
    } //TODO remove

    return QGraphicsRectItem::itemChange(change, value);
}

void GObjectHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // This has to be before the superclass method call
    if (!isSelected())
    {
        obj->_handleAboutToBeSelected = true;
        obj->_handleSelected = true;
    }
    QGraphicsRectItem::mousePressEvent(event);
}

QPointF GObjectHandle::getCenterPos() const
{
    return pos() + QPointF{1, 1};
}

void GObjectHandle::setCenterPos(const QPointF &position)
{
    setPos(position + QPointF{1, 1});
}
