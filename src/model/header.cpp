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

void Header::update()
{
    clear();
}

void Header::init()
{

}

QRectF Header::getContentArea() const
{
    return contentArea;
}

void Header::clear()
{

}
