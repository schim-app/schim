#include "header.h"

#include "line.h"

Header::Header() { }

Header::Header(CompositeObject &&obj)
    : Header()
{
    append(obj);
    obj.clear();
}

Header::Header(CompositeObject *obj)
    : Header()
{
    append(*obj);
    obj->clear();
    delete obj; //TODO maybe not do this here
}

Header::Header(const Header &obj)
    : CompositeObject(obj)
{
    contentArea = obj.contentArea;
}

void Header::update()
{
    clear();
}

void Header::init()
{

}

void Header::setContentArea(const QRectF &rect)
{
    contentArea = rect;
}

QRectF Header::getContentArea() const
{
    return contentArea;
}

Header *Header::clone() const
{
    return new Header(*this);
}

void Header::clear()
{

}
