#include "component.h"

Component::Component()
    : CompositeObject()
{ }

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

Component *Component::absorb(CompositeObject *obj)
{
    Component *retVal = new Component();
    retVal->append(*obj);
    obj->clear();
    delete obj;
    return retVal;
}
