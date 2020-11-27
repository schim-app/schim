#ifndef OBJECTARRAY_H
#define OBJECTARRAY_H

#include "compositeobject.h"

class ObjectArray : public CompositeObject
{
public:
    ObjectArray(Object *base);
    virtual ~ObjectArray();

    /** @brief Set the base item of the array. */
    virtual void rebase(int index);
    virtual Object *generate(int index) const = 0;

    virtual int getCount();
    Object* getBaseObject();

protected:
    // HELPER METHODS
    /** @brief Remove and delete all items apart from the base item. */
    void clear();

protected:
    Object *baseObj{};
};

#endif // OBJECTARRAY_H
