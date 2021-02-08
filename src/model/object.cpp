#include "object.h"

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

void Object::setPos(const QPointF &pos) { Q_UNUSED(pos) }

void Object::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}

void Object::setParent(CompositeObject *parent)
{
    this->parent = parent;
}

void Object::setProperty(const QString &, const QString &)
{
}

QString Object::getProperty(const QString &) const
{
    return "";
}

bool Object::operator==(const Object &obj) const
{
    return getPos() == obj.getPos();
}

bool Object::operator!=(const Object &obj) const
{
    return !(*this == obj);
}
