#include "gheader.h"

GHeader::GHeader(Header *obj)
    : GObject(obj)
{
    for (auto *child : obj->children)
        GObject::assign(child)->setParentItem(this);
}
