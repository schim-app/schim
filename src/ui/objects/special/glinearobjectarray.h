#ifndef GLINEAROBJECTARRAY_H
#define GLINEAROBJECTARRAY_H

#include "ui/objects/gcompositeobject.h"
#include "model/special/linearobjectarray.h"

class GLinearObjectArray : public GCompositeObject
{
public:
    GLinearObjectArray(LinearObjectArray *obj);

    LinearObjectArray *get();
    const LinearObjectArray *get() const;

    void reload() override;
    void apply() override;

};

#endif // GLINEAROBJECTARRAY_H
