#include "component.h"

Component::Component()
    : CompositeObject()
{ }

Component::Component(QSharedPointer<Device> device)
    : CompositeObject(), device(device) { }

Component::Component(CompositeObject *obj)
{
    append(*obj);
    obj->clear();
    delete obj;
}
