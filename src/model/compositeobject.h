#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "object.h"

#include <QList>

/**
 * An object that consists of other child objects.
 * This is the most general representation of an object.
 */
class CompositeObject : public Object
{

protected:

    QList<Object*> children;

public:
    CompositeObject();
    ~CompositeObject();

    virtual void addChild(Object *child);

    friend class GCompositeObject;
};

#endif // COMPOSITEOBJECT_H
