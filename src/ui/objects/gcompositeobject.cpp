#include "gcompositeobject.h"

#include "model/text.h"
#include "ui/sheetscene.h"

#include <iostream>

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

// SETTERS

void GCompositeObject::setCosmetic(bool cosmetic)
{
    GObject::setCosmetic(cosmetic);
    for (auto *item : childItems())
        static_cast<GObject*>(item)->setCosmetic(cosmetic);
}

// MISCELLANEOUS

void GCompositeObject::applyToModel()
{
    get()->setPos(pos());
}

void GCompositeObject::reloadFromModel()
{
    setPos(get()->getPos());
    for (auto *child : childItems())
        static_cast<GObject*>(child)->reloadFromModel();
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

// OVERRIDE QGraphicsObject

QPainterPath GCompositeObject::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

QRectF GCompositeObject::boundingRect() const
{
    QRectF rect; // The return value
    // Some helper variables
    bool initialized = false;
    auto &constituents = get()->getConstituents();
    int j = 0;

    for (int i = 0; i < childItems().size() && j < constituents.count(); ++i)
    {
        auto *child = dynamic_cast<GObject*>(childItems()[i]);
        if (child == nullptr) continue; // Looking for Object children
        if (child->get() == constituents[j])
        { // Child is a constituent
            if (!initialized) // The initial guess
            {
                rect = child->boundingRect().translated(child->pos());
                initialized = true;
            }
            else // Expand the rect with this child's boundingRect
                rect = rect.united(child->boundingRect().translated(child->pos()));
            ++j;
        }
    }
    return rect;
}
