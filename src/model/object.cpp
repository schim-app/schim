#include "object.h"

QPointF Object::getPos() const
{
    return pos;
}

void Object::setPos(const QPointF &pos)
{
    this->pos = pos;
}
