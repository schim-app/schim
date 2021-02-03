#ifndef LINEAROBJECTARRAY_H
#define LINEAROBJECTARRAY_H

#include "model/objectarray.h"

class LinearObjectArray : public ObjectArray
{
public:
    LinearObjectArray(Object *base, float deltaX, float deltaY, int count);
    LinearObjectArray(const LinearObjectArray &obj);

    int getCount() override;

    void generate() override;

    Object *clone() const override;

private:
    float deltaX = 1, deltaY = 0;
    int count = 0;
};

#endif // LINEAROBJECTARRAY_H
