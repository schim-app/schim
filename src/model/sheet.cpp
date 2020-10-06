#include "sheet.h"

Sheet::Sheet()
{
}

Sheet::~Sheet()
{
    delete header;
}

void Sheet::addObject(Object *obj)
{
    objects.append(obj);
}

float Sheet::getWidth() const
{
    return width;
}

float Sheet::getHeight() const
{
    return height;
}

QString Sheet::getTitle() const
{
    return title;
}

Header *Sheet::getHeader() const
{
    return header;
}

void Sheet::setTitle(const QString &title)
{
    this->title = title;
}
