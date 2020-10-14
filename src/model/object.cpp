#include "object.h"

Object::Object()
{

}

Object::~Object()
{

}

QPointF Object::getPos() const
{
    return pos;
}

void Object::setPos(const QPointF &pos)
{
    this->pos = pos;
}
