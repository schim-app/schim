#include "gheader.h"

#include "ui/sheetscene.h"

GHeader::GHeader(Header *obj)
    : GCompositeObject(obj)
{
    setFlags(0);
    setAcceptHoverEvents(false);
}

Header *GHeader::get()
{
    return (Header*) obj;
}

const Header *GHeader::get() const
{
    return (Header*) obj;
}

QVariant GHeader::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    // When the header is added to a scene, add all its children as well and
    // set their parent to this
    if (change == ItemSceneHasChanged && scene() != nullptr)
        for (auto *child : *get())
        {
            auto *assignee = GObject::assign(child);
            scene()->addItem(assignee);
            assignee->reload();
            assignee->setParentItem(this);
            assignee->setEnabled(false);
            assignee->setFlag(ItemIsMovable, false);
            assignee->setFlag(ItemIsSelectable, false);
        }
    return value;
}
