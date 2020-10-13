#include "gheader.h"

#include "ui/sheetscene.h"

GHeader::GHeader(Header *obj)
    : GObject(obj)
{
    setFlags(0);
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
        for (auto *child : get()->children)
        {
            auto *assignee = GObject::assign(child);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
            assignee->setEnabled(false);
        }
    return value;
}
