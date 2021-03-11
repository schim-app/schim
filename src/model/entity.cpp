#include "entity.h"

// CONSTRUCTORS

Entity::Entity(Entity *parent)
    : parent(parent) { }

// GETTERS

QString Entity::getName() const
{
    return name;
}

QString Entity::getFileName() const
{
    return fileName;
}

VariableSet Entity::getVariables() const
{
    // TODO impl
    VariableSet vars = getLocalVariables();
    if (getParent())
        vars.append(getParent()->getVariables());

    return variables;
}

VariableSet &Entity::getLocalVariables()
{
    return variables;
}

VariableSet Entity::getLocalVariables() const
{
    return variables;
}

Entity *Entity::getParent() const
{
    return parent;
}

// SETTERS

void Entity::setName(const QString &name)
{
    this->name = name;
}

void Entity::setFileName(const QString &name)
{
    this->fileName = name;
}

void Entity::setVariables(const VariableSet &vars)
{
    variables = vars;
}

void Entity::addVariable(const Variable &variable)
{
    variables.append(variable);
}

void Entity::addVariables(const VariableSet &variables)
{
    this->variables.append(variables);
}

void Entity::setParent(Entity *parent)
{
    this->parent = parent;
}
