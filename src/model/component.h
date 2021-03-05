#ifndef COMPONENT_H
#define COMPONENT_H

#include "model/compositeobject.h"

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

    // MISCELLANEOUS
    /** @brief Steal everything from `obj` and delete it. */
    static Component *absorb(CompositeObject *obj);

    // OPERATORS
    virtual bool operator==(const Component &obj) const;
    virtual bool operator!=(const Component &obj) const;

private:
    QSharedPointer<Device> device;
};

#endif // COMPONENT_H
