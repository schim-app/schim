#include "compositeobject.h"

CompositeObject::CompositeObject()
    : Object()
{

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
