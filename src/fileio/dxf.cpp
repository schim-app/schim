#include "dxf.h"

#include <dxflib/dl_dxf.h>

DXFFilter::DXFFilter(CompositeObject *obj)
{
    this->obj = obj;
}

void DXFFilter::addLine(const DL_LineData &d)
{
    obj->add(new Line(d.x1, d.y1, d.x2, d.y2));
}

CompositeObject *dxfParseCompositeObject(const QString &filename)
{
    DL_Dxf *dxf = new DL_Dxf;
    auto *obj = new CompositeObject;
    DXFFilter filter(obj);
    if (!dxf->in(filename.toStdString(), &filter))
        throw std::logic_error((filename + " is not a valid DXF file").toStdString());
    delete dxf;

    return obj;
}

CompositeObject *dxfParseCompositeObject(const std::string &contents)
{
    std::stringstream stream(contents);

    DL_Dxf *dxf = new DL_Dxf;
    auto *obj = new CompositeObject;
    DXFFilter filter(obj);
    if (!dxf->in(stream, &filter))
        throw std::logic_error("Specified content is not a valid DXF format");
    delete dxf;

    return obj;
}
