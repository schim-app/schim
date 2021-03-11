#include "header.h"

#include "line.h"

// CONSTRUCTORS

Header::Header(CompositeObject &&obj)
    : Header()
{
    add(obj.getConstituents());
    obj.getConstituents().clear();
}

Header::Header(CompositeObject *obj)
    : Header()
{
    add(obj->getConstituents());
    obj->getConstituents().clear();
    delete obj; //TODO maybe not do this here
}

Header::Header(const Header &obj)
    : CompositeObject(obj), contentArea(obj.contentArea) { }

Header *Header::clone() const
{
    return new Header(*this);
}

// GETTERS

QRectF Header::getContentArea() const
{
    return contentArea;
}

// SETTERS

void Header::setContentArea(const QRectF &rect)
{
    contentArea = rect;
}

// OPERATORS

bool Header::operator==(const Header &obj) const
{
    return CompositeObject::operator==(obj)
            && obj.contentArea == contentArea
            && obj.displayColumns == displayColumns
            && obj.displayRows == displayColumns
            && obj.columnMarkers == columnMarkers
            && obj.rowMarkers == rowMarkers;
}

bool Header::operator!=(const Header &obj) const
{
    return !(*this == obj);
}
