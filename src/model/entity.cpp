#include "entity.h"

QString Entity::getName() const
{
    return name;
}

QString Entity::getFileName() const
{
    return filename;
}

void Entity::setName(const QString &name)
{
    this->name = name;
}

void Entity::setFileName(const QString &name)
{
    this->filename = name;
}
