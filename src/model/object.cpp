#include "object.h"

QPointF Object::getPos() const
{
    return {};
}

Sheet *Object::getSheet() const
{
    return sheet;
}

void Object::setPos(const QPointF &pos) { Q_UNUSED(pos) }

void Object::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}
