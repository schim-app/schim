#include "dxf.h"

DXFFilter::DXFFilter(CompositeObject *obj)
{
    this->obj = obj;
}

void DXFFilter::addLine(const DL_LineData &d)
{
    obj->addChild(new Line(d.x1, d.y1, d.x2, d.y2));
}
