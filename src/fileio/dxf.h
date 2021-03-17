/// @file dxf.h
#ifndef DXFFILTER_H
#define DXFFILTER_H

#include <dxflib/dl_creationadapter.h>

#include "model/line.h"
#include "model/object.h"
#include <model/compositeobject.h>

/**
 * @brief A class that facilitates conversion to and from DXF files
 *
 * For details refer to the dxflib documentation.
 */
class DXFFilter : public DL_CreationAdapter
{
public:
    explicit DXFFilter(CompositeObject *obj);

    virtual void addLine(const DL_LineData &d);
private:
    CompositeObject *obj;
};

CompositeObject *dxfParseCompositeObject(const QString &filename);
CompositeObject *dxfParseCompositeObject(const std::string &contents);

#endif // DXFFILTER_H
