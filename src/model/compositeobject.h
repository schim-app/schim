#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "object.h"

#include <QList>

/**
 * @brief An object that consists of other child objects.
 *
 * This is the most general representation of an object. Destruction
 * of its child objects should always be performed in the destructor.
 * This class inherits QList<Object*> so we don't have to
 * reimplement the convenience functions that QList already provides.
 */
class CompositeObject : public Object, public QList<Object*>
{

protected:

public:
    CompositeObject();
    ~CompositeObject();

    friend class GCompositeObject;
};

#endif // COMPOSITEOBJECT_H
