#include "compositeobject.h"

#include "model/sheet.h"

// CONSTRUCTORS

CompositeObject::CompositeObject()
    : Object() { }

CompositeObject::CompositeObject(const CompositeObject &obj)
    : Object()
{
    variables = obj.variables;
    constituents.reserve(obj.getConstituents().size());
    for (Object *object : obj.getConstituents())
        add(object->clone());
    file = obj.file;
}

CompositeObject::~CompositeObject()
{
    for (Object *child : getConstituents())
        delete child;
}

Object *CompositeObject::clone() const
{
    return new CompositeObject(*this);
}

// GETTERS

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

QList<Object *> &CompositeObject::getConstituents()
{
    return constituents;
}

QList<Object *> CompositeObject::getConstituents() const
{
    return constituents;
}

// SETTERS

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

void CompositeObject::setLocalVariables(const VariableSet &vars)
{
    variables.clear();
    for (const auto &var : vars)
        if (var.getTrueName() != "")
            variables.append(var);
}

void CompositeObject::setSourceFile(const QString &filename)
{
    this->file = filename;
    // TODO maybe load the object from the file...
}

void CompositeObject::addVariable(const Variable &variable)
{
    for (const auto &var : variables)
        if (var == variable)
            return;
    variables.append(variable);
}

void CompositeObject::add(Object *object)
{
    getConstituents().append(object);
    object->setParent(this);
    object->setSheet(sheet);
}

void CompositeObject::add(const QList<Object *> &list)
{
    getConstituents().reserve(list.size());
    for (auto *obj : list)
    {
        obj->setParent(this);
        obj->setSheet(sheet);
    }
    getConstituents().append(list);
}

void CompositeObject::remove(Object *obj)
{
    constituents.removeOne(obj);
}

// OPERATORS

bool CompositeObject::operator==(const CompositeObject &obj) const
{
    if (file == obj.file) // Objects are taken from the same file
        return true;

    if (obj.getConstituents().size() != getConstituents().size()
            || obj.variables != variables)
        return false; // Objects do not have same number of children

    for (int i = 0; i < getConstituents().size(); ++i)
        if (getConstituents().at(i) != obj.getConstituents().at(i))
            return false; // A child object is different

    return Object::operator==(obj) && obj.pos == pos;
}

bool CompositeObject::operator!=(const CompositeObject &obj) const
{
    return !(*this == obj);
}
