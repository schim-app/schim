#include "sheet.h"

Sheet::Sheet()
{
}

void Sheet::addObject(Object *obj)
{
    objects.append(obj);
}

QString Sheet::getTitle() const
{
    return title;
}

void Sheet::setTitle(const QString &title)
{
    this->title = title;
}
