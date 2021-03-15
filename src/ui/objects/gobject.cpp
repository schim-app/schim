#include "gobject.h"

#include "gheader.h"
#include "gcomponent.h"
#include "gline.h"
#include "grect.h"
#include "gtext.h"
#include "gterminal.h"
#include "special/glinearobjectarray.h"

#include "global.h"

#include "model/header.h"
#include "model/component.h"
#include "model/rect.h"
#include "model/text.h"
#include "model/terminal.h"
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

SheetScene *GObject::getSheetScene() const
{
    return dynamic_cast<SheetScene*>(scene());
}

// SETTERS

void GObject::setCosmetic(bool cosmetic)
{
    this->cosmetic = cosmetic;
}

// OBJECT EDITING

void GObject::reloadFromModel() { }

void GObject::applyToModel() { }

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
    if_cast_return(Line, obj);
    if_cast_return(Rect, obj);
    if_cast_return(Text, obj);
    if_cast_return(Terminal, obj);
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
    if (flags() & ItemIsSelectable)
    {
        hovered = true;
        getSheetScene()->hoveredItem = getOldestParent();
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void GObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (flags() & ItemIsSelectable)
        hovered = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void GObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
        getSheetScene()->setSnapCursorGuides(true);
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
    else if (change == ItemSceneHasChanged || change == ItemParentHasChanged)
    {
        if (value.isNull())
        { // Unselect and unhover the item when it leaves the scene
            setSelected(false);
            hovered = false;
        }
        else
            reloadFromModel();
    }
    else if (change == ItemPositionChange && getSheetScene())
        return getSheetScene()->snap(value.toPointF());
    else if (change == ItemPositionHasChanged)
        applyToModel();

    return QGraphicsItem::itemChange(change, value);
}

// OVERRIDE QGraphicsItem

QRectF GObject::boundingRect() const
{
    return childrenBoundingRect();
}

void GObject::paint(QPainter *painter, const QStyleOptionGraphicsItem*,
                    QWidget *)
{
    auto pen = painter->pen();
    pen.setCosmetic(cosmetic);

    // The order is important
    if (isSelected() || (parentItem() && parentItem()->isSelected()))
        pen.setColor(qApp->palette().color(QPalette::Highlight));
    else if (isHovered() || (parentItem() &&  parentItem()->isHovered()))
        // TODO the color source may be a little stupid, but it looks reasonably nice
        pen.setColor(qApp->palette().color(QPalette::LinkVisited));
    else
        // The default color is the theme's text color
        pen.setColor(qApp->palette().color(QPalette::Text));

    painter->setPen(pen);
}

GCompositeObject *GObject::parentItem() const
{
    return static_cast<GCompositeObject*>(QGraphicsItem::parentItem());
}

Entity *GObject::getModelParent() const
{
    if (parentItem())
        return parentItem()->get();
    else if (getSheetScene())
        return getSheetScene()->getSheet();
    else
        return nullptr;
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
    return static_cast<SheetScene*>(QGraphicsItem::scene());
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
