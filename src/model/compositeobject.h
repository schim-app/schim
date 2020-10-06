#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "object.h"

#include <QList>

class CompositeObject : public Object
{

protected:

    QList<Object*> children;

public:
    CompositeObject();
    ~CompositeObject();

    virtual void addChild(Object *child);
};

#endif // COMPOSITEOBJECT_H
