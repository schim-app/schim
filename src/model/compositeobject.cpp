#include "compositeobject.h"

#include "model/sheet.h"

CompositeObject::CompositeObject()
    : Object() { }

CompositeObject::CompositeObject(const CompositeObject &obj)
    : Object(), QList<Object *>()
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

VariableSet CompositeObject::getVariables() const
{
    VariableSet vars = variables;
    if (parent)
        vars.append(parent->getVariables());
    else if (sheet)
        vars.append(sheet->getVariables());

    return vars;
}

VariableSet &CompositeObject::getLocalVariables()
{
    return variables;
}

VariableSet CompositeObject::getLocalVariables() const
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
