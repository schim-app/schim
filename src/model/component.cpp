#include "component.h"

// CONSTRUCTORS

Component::Component(QSharedPointer<Device> device)
    : CompositeObject(), device(device) { }

Component::Component(const Component &obj)
    : CompositeObject(obj)
{
    //TODO what to do with device
}

Component::~Component()
{
    qDeleteAll(texts);
    qDeleteAll(terminals);
}

Object *Component::clone() const
{
    return new Component(*this);
}

// GETTERS

QList<Text *> &Component::getTexts()
{
    return texts;
}

QList<Terminal *> &Component::getTerminals()
{
    return terminals;
}

// SETTERS

void Component::addText(Text *text)
{
    texts.append(text);
}

void Component::addTexts(const QList<Text*> &texts)
{
    // TODO children->setParent?
    this->texts.append(texts);
}

void Component::addTerminal(Terminal *terminal)
{
    terminals.append(terminal);
    terminal->setParent(this);
}

void Component::addTerminals(const QList<Terminal *> &terminals)
{
    for (auto *t : terminals)
        t->setParent(this);
    this->terminals.append(terminals);
}

void Component::remove(Object *obj)
{
    if (dynamic_cast<Text*>(obj) &&
            texts.removeOne(static_cast<Text*>(obj)))
        return;
    else if (dynamic_cast<Terminal*>(obj) &&
            terminals.removeOne(static_cast<Terminal*>(obj))) return;
    else CompositeObject::remove(obj);
}

// MISCELLANEOUS

Component *Component::absorb(CompositeObject *obj)
{
    Component *retVal = new Component;
    retVal->add(obj->getConstituents());
    obj->getConstituents().clear();
    delete obj;
    return retVal;
}

// OPERATORS

bool Component::operator==(const Component &obj) const
{
    // TODO Customize for this type of object
    return CompositeObject::operator==(obj);
}

bool Component::operator!=(const Component &obj) const
{
    return !(*this == obj);
}
