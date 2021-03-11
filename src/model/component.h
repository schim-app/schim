#ifndef COMPONENT_H
#define COMPONENT_H

#include "model/compositeobject.h"
#include "terminal.h"
#include "text.h"

#include <QList>
#include <QSharedPointer>

class Device;

class Component : public CompositeObject
{
public:
    // CONSTRUCTORS
    Component() = default;
    Component(QSharedPointer<Device> device);
    Component(const Component &obj);
    Object *clone() const override;

    // GETTERS
    QList<Text*> &getTexts();
    QList<Terminal*> &getTerminals();

    // SETTERS
    void addText(Text *text);
    void addTexts(const QList<Text*> &texts);
    void addTerminal(Terminal *terminal);
    void addTerminals(const QList<Terminal*> &terminals);
    void remove(Object *obj);

    // MISCELLANEOUS
    /**
     * @brief Steal everything from `obj` and delete it.
     *
     * This creates a new component containing all constituents from `obj`, and
     * then destroys `obj`.
     */
    static Component *absorb(CompositeObject *obj);

    // OPERATORS
    virtual bool operator==(const Component &obj) const;
    virtual bool operator!=(const Component &obj) const;

private:
    QList<Text*> texts;
    QList<Terminal*> terminals;
    QSharedPointer<Device> device;
};

#endif // COMPONENT_H
