#ifndef GLINEAROBJECTARRAY_H
#define GLINEAROBJECTARRAY_H

#include "ui/objects/gcompositeobject.h"
#include "model/special/linearobjectarray.h"

class GLinearObjectArray : public GCompositeObject
{
public:
    explicit GLinearObjectArray(LinearObjectArray *obj);

    LinearObjectArray *get();
    const LinearObjectArray *get() const;

    void reloadFromModel() override;
    void applyToModel() override;
};

#endif // GLINEAROBJECTARRAY_H
