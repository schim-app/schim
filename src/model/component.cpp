#include "component.h"

// CONSTRUCTORS

Component::Component(QSharedPointer<Device> device)
    : CompositeObject(), device(device) { }

Component::Component(const Component &obj)
    : CompositeObject(obj)
{
    //TODO what to do with device
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
    text->setParent(this);
    text->setSheet(sheet);
}

void Component::addTexts(const QList<Text*> &texts)
{
    for (auto *t : texts)
    {
        t->setParent(this);
        t->setSheet(sheet);
    }
    this->texts.append(texts);
}

void Component::addTerminal(Terminal *terminal)
{
    terminals.append(terminal);
    terminal->setParent(this);
    terminal->setSheet(sheet);
}

void Component::addTerminals(const QList<Terminal *> &terminals)
{
    for (auto *t : terminals)
    {
        t->setParent(this);
        t->setSheet(sheet);
    }
    this->terminals.append(terminals);
}

void Component::remove(Object *obj)
{
    if (dynamic_cast<Text*>(obj) && texts.removeOne((Text*) obj)) return;
    else if (dynamic_cast<Terminal*>(obj) &&
            terminals.removeOne((Terminal*) obj)) return;
    else CompositeObject::remove(obj);
}

// MISCELLANEOUS

Component *Component::absorb(CompositeObject *obj)
{
    Component *retVal = new Component();
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
