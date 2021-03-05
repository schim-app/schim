#include "gobject.h"

#include "gheader.h"
#include "gcomponent.h"
#include "gline.h"
#include "grect.h"
#include "gtext.h"
#include "special/glinearobjectarray.h"

#include "global.h"

#include "model/header.h"
#include "model/component.h"
#include "model/rect.h"
#include "model/text.h"
#include "model/special/linearobjectarray.h"

#include <QGraphicsSceneMouseEvent>
#include <QApplication>

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
        qDeleteAll(*handles);
        delete handles;
    }
    qDeleteAll(childItems());
}

// GETTERS

Object *GObject::get()
{
    return obj;
}

const Object *GObject::get() const
{
    return obj;
}

bool GObject::isHovered() const
{
    return hovered;
}

GObject *GObject::getOldestParent()
{
    if (parentItem() != nullptr)
        return parentItem()->getOldestParent();
    else
        return this;
}

// SETTERS

void GObject::setCosmetic(bool cosmetic)
{
    this->cosmetic = cosmetic;
}

// OVERRIDE

QRectF GObject::boundingRect() const
{
    return childrenBoundingRect();
}

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    auto pen = painter->pen();
    // The default color (unless overriden) is black
    pen.setColor(qApp->palette().color(QPalette::Text));
    pen.setCosmetic(cosmetic);

    // The order is important
    if (isSelected() || (parentItem() && parentItem()->isSelected()))
        pen.setColor(qApp->palette().color(QPalette::Highlight));
    else if (isHovered() || (parentItem() &&  parentItem()->isHovered()))
        // TODO the color source may be a little stupid, but it looks reasonably nice
        pen.setColor(qApp->palette().color(QPalette::LinkVisited));

    painter->setPen(pen);
}

GCompositeObject *GObject::parentItem()
{
    return (GCompositeObject *) QGraphicsItem::parentItem();
}

SheetScene *GObject::scene()
{
    return (SheetScene*) QGraphicsItem::scene();
}

// FOR EDITING THE OBJECT

void GObject::reload() { }

void GObject::apply() { }

void GObject::showHandles(bool show)
{
    if (show && handles == nullptr)
        moveHandlesAbove();
    if (!show && handles != nullptr)
    {
        qDeleteAll(*handles);
        delete handles;
        handles = nullptr;
    }
}

void GObject::handleChanged(GObjectHandle *) { }

// STATIC

GObject *GObject::assign(Object *obj)
{
#define if_cast_return(TYPE, object) if (dynamic_cast<TYPE*>(object)) return new G##TYPE((TYPE*)object)
    Header *hdr = dynamic_cast<Header*>(obj);

    if_cast_return(Line, obj);
    if_cast_return(Rect, obj);
    if_cast_return(Text, obj);
    if_cast_return(Header, obj);
    if_cast_return(Component, obj);
    if_cast_return(LinearObjectArray, obj);
    if_cast_return(CompositeObject, obj);

#undef if_cast_return
    return nullptr;
}

// EVENTS

void GObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    if (flags() & ItemIsSelectable)
    {
        hovered = true;
        scene()->hoveredItem = getOldestParent();
    }
}

void GObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    if (flags() & ItemIsSelectable)
        hovered = false;
}

void GObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        scene()->setSnapCursorGuides(true);
    QGraphicsItem::mouseMoveEvent(event);
}

void GObject::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

QVariant GObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemZValueHasChanged)
        moveHandlesAbove();
    else if (change == ItemSelectedHasChanged)
        showHandles(value.toBool());
    else if (change == ItemSceneHasChanged)
    {
        if (value.isNull())
        { // Unselect and unhover the item when it leaves the scene
            setSelected(false);
            hovered = false;
        }
        else
            reload();
    }
    else if (change == ItemPositionChange && scene())
        return scene()->snap(value.toPointF());
    else if (change == ItemPositionHasChanged)
        apply();

    return QGraphicsItem::itemChange(change, value);
}

// HELPER METHODS

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
    _pen.setColor(qApp->palette().color(QPalette::Highlight));
    setPen(_pen);
    setBrush(qApp->palette().color(QPalette::Highlight));

}

SheetScene *GObjectHandle::scene()
{
    return (SheetScene*) QGraphicsItem::scene();
}

// EVENTS

QVariant GObjectHandle::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged)
        obj->handleChanged(this);

    return QGraphicsRectItem::itemChange(change, value);
}

void GObjectHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        _dragStartPos = scene()->snap(event->scenePos());
        event->accept();
    }
    else
        QGraphicsRectItem::mousePressEvent(event);
}

void GObjectHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto delta = scene()->snap(event->scenePos()) - _dragStartPos;
        moveBy(delta.x(), delta.y());
        _dragStartPos = scenePos();

        scene()->setSnapCursorGuides(true);
        event->accept();
    }
    else
        QGraphicsRectItem::mouseMoveEvent(event);
}
