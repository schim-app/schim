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
    file = obj.file;
}

CompositeObject::~CompositeObject()
{
    for (Object *child : *this)
        delete child;
}

QPointF CompositeObject::getPos() const
{
    return pos;
}

QString CompositeObject::getValue(const QString &name, bool *exists) const
{
    auto var = Variable::find(getVariables(), name);
    if (exists != nullptr)
        *exists = !var.names.empty();
    return var.value;
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

QString CompositeObject::getSourceFile() const
{
    return file;
}

void CompositeObject::setPos(const QPointF &pos)
{
    this->pos = pos;
}

void CompositeObject::setValue(const QString &name, const QString &value)
{
    for (auto &var : variables)
        if (var.names.contains(name))
        {
            var.value = value;
            return;
        }
    addVariable({{name}, "", value});
}

void CompositeObject::addVariable(const Variable &variable)
{
    for (const auto &var : variables)
        if (var == variable)
            return;
    variables.append(variable);
}

void CompositeObject::setLocalVariables(const VariableSet &vars)
{
    variables.clear();
    for (auto var : vars)
        if (var.getTrueName() != "")
            variables.append(var);
}

void CompositeObject::setSourceFile(const QString &filename)
{
    this->file = filename;
    // TODO maybe load the object from the file...
}

Object *CompositeObject::clone() const
{
    return new CompositeObject(*this);
}

void CompositeObject::append(Object *object)
{
    QList::append(object);
    object->parent = this;
    object->sheet = sheet;
}

void CompositeObject::append(const QList<Object *> &list)
{
    for (auto *obj : list)
    {
        obj->parent = this;
        obj->sheet = sheet;
    }
    QList::append(list);
}
