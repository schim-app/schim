#ifndef OBJECTARRAY_H
#define OBJECTARRAY_H

#include "compositeobject.h"

class ObjectArray : public CompositeObject
{
public:
    ObjectArray(Object *base);
    ObjectArray(const ObjectArray &obj);
    virtual ~ObjectArray();

    /** @brief Set the base item of the array. */
    virtual void rebase(int index);
    virtual void generate() = 0;

    virtual int getCount();
    Object* getBaseObject();

protected:
    Object *baseObj{};
};

#endif // OBJECTARRAY_H
