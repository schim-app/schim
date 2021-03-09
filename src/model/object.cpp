#include "object.h"

// GETTERS

QPointF Object::getPos() const
{
    return {};
}

Sheet *Object::getSheet() const
{
    return sheet;
}

CompositeObject *Object::getParent() const
{
    return parent;
}

// SETTERS

void Object::setPos(const QPointF &) { }

void Object::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}

void Object::setParent(CompositeObject *parent)
{
    this->parent = parent;
}

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
