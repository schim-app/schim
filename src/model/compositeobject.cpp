#include "compositeobject.h"

CompositeObject::CompositeObject()
    : Object() { }

CompositeObject::CompositeObject(const CompositeObject &obj)
    : Object()
{
    variables = obj.variables;
    this->reserve(obj.size());
    for (Object *object : obj)
        this->append(object->clone());
}

CompositeObject::~CompositeObject()
{
    for (Object *child : *this)
        delete child;
}

QString CompositeObject::getValue(const QString &name, bool *exists) const
{
    *exists = true;
    for (auto &var : variables)
        if (var.name == name)
            return var.value;
    *exists = false;
    return "";
}

QList<Variable> CompositeObject::getVariables() const
{
    return variables;
}

void CompositeObject::setValue(const QString &name, const QString &value)
{
    for (auto &var : variables)
        if (var.name == name)
        {
            var.value = value;
            return;
        }
    addVariable({name, "", value});
}

void CompositeObject::addVariable(const Variable &variable)
{
    variables.append(variable);
}

Object *CompositeObject::clone() const
{
    return new CompositeObject(*this);
}
