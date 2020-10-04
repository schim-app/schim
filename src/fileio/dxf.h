#ifndef DXFFILTER_H
#define DXFFILTER_H

#include <dxflib/dl_creationadapter.h>

#include "model/line.h"
#include "model/object.h"

class DXFFilter : public DL_CreationAdapter
{
    Object *obj;
public:

    DXFFilter(Object *obj);

    virtual void addLine(const DL_LineData &d);
};

#endif // DXFFILTER_H
