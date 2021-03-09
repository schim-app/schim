#include "gcompositeobject.h"

#include "ui/sheetscene.h"

#include "model/text.h"

// CONSTRUCTORS

GCompositeObject::GCompositeObject(CompositeObject *obj)
    : GObject(obj) { }

// GETTERS

CompositeObject *GCompositeObject::get()
{
    return (CompositeObject*) obj;
}

const CompositeObject *GCompositeObject::get() const
{
    return (CompositeObject*) obj;
}

// OVERRIDE QGraphicsObject

QPainterPath GCompositeObject::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

// SETTERS

void GCompositeObject::setCosmetic(bool cosmetic)
{
    GObject::setCosmetic(cosmetic);
    for (auto *item : childItems())
        static_cast<GObject*>(item)->setCosmetic(cosmetic);
}

// MISCELLANEOUS

void GCompositeObject::apply()
{
    get()->setPos(pos());
}

void GCompositeObject::reload()
{
    setPos(get()->getPos());
    for (auto *child : childItems())
        static_cast<GObject*>(child)->reload();
}

// EVENTS

QVariant GCompositeObject::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // When the object is added to a scene, add all its children as well and
    // set their parent to this
    if (change == ItemSceneHasChanged && scene() != nullptr)
        for (auto *child : get()->getConstituents())
        {
            auto *assignee = GObject::assign(child);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
            assignee->setFlags({});
        }
    else
        return GObject::itemChange(change, value);

    return value;
}
