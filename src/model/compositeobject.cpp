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

QString CompositeObject::getFileName() const
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

void CompositeObject::setSourceFile(const QString &filename)
{
    this->file = filename;
    // TODO maybe load the object from the file...
}

void CompositeObject::add(Object *object)
{
    getConstituents().append(object);
    // TODO object->setParent?
}

void CompositeObject::add(const QList<Object *> &list)
{
    getConstituents().reserve(list.size());
    // TODO children->setParent?
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
