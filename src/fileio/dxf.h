#ifndef DXFFILTER_H
#define DXFFILTER_H

#include <dxflib/dl_creationadapter.h>

#include "model/line.h"
#include "model/object.h"
#include <model/compositeobject.h>

class DXFFilter : public DL_CreationAdapter
{
    CompositeObject *obj;
public:

    DXFFilter(CompositeObject *obj);

    virtual void addLine(const DL_LineData &d);
};

#endif // DXFFILTER_H
