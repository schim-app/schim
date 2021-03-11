#include "gheader.h"

#include "ui/sheetscene.h"

// CONSTRUCTORS

GHeader::GHeader(Header *obj)
    : GCompositeObject(obj)
{
    setFlags({});
    setAcceptHoverEvents(false);
}

// GETTERS

Header *GHeader::get()
{
    return (Header*) obj;
}

const Header *GHeader::get() const
{
    return (Header*) obj;
}
