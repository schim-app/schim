#include "object.h"

// GETTERS

QPointF Object::getPos() const
{
    return {};
}

// SETTERS

void Object::setPos(const QPointF &) { }

// SET/GET PROPERTY
void Object::setProperty(const QString &, const QString &)
{
}
QString Object::getProperty(const QString &) const
{
    return "";
}

// OPERATORS

bool Object::operator==(const Object &obj) const
{
    return getPos() == obj.getPos();
}

bool Object::operator!=(const Object &obj) const
{
    return !(*this == obj);
}
