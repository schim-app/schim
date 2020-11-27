#ifndef GLINEAROBJECTARRAY_H
#define GLINEAROBJECTARRAY_H

#include "ui/objects/gcompositeobject.h"
#include "model/special/linearobjectarray.h"

class GLinearObjectArray : public GObject
{
public:
    GLinearObjectArray(LinearObjectArray *obj);
    ~GLinearObjectArray();

    LinearObjectArray *get();
    const LinearObjectArray *get() const;

    void reload() override;
    void apply() override;

private:
    void clear();
};

#endif // GLINEAROBJECTARRAY_H
