#ifndef COMPONENT_H
#define COMPONENT_H

#include "model/compositeobject.h"

#include <QSharedPointer>

class Device;

class Component : public CompositeObject
{
public:
    Component();
    Component(QSharedPointer<Device> device);
    /**
     * @brief Steal everything from `obj` and delete it.
     */
    Component(CompositeObject *obj);
    Component(const Component &obj);

    Object *clone() const override;

private:
    QSharedPointer<Device> device;
};

#endif // COMPONENT_H
