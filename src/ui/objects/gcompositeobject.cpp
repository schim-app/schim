#include "gcompositeobject.h"

#include "ui/sheetscene.h"

GCompositeObject::GCompositeObject(CompositeObject *obj)
    : GObject(obj) { }

CompositeObject *GCompositeObject::get()
{
    return (CompositeObject*) obj;
}

const CompositeObject *GCompositeObject::get() const
{
    return (CompositeObject*) obj;
}

QVariant GCompositeObject::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // When the object is added to a scene, add all its children as well and
    // set their parent to this
    if (change == ItemSceneHasChanged && scene() != nullptr)
        for (auto *child : get()->children)
        {
            auto *assignee = GObject::assign(child);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
            assignee->setFlags(0);
        }
    return value;
}

QPainterPath GCompositeObject::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

QRectF GCompositeObject::boundingRect() const
{
    return childrenBoundingRect();
}
