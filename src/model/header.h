#ifndef HEADER_H
#define HEADER_H

#include "object.h"
#include "rect.h"
#include "compositeobject.h"

class Header : public CompositeObject
{
    // Both of these strings represent bash-readable arrays
    QString columnMarkers = "{0..9}", rowMarkers = "{A..F}";
    bool displayColumns = true, displayRows = true;

    friend class GHeader;

public:
    Header();
};

class DefaultHeader : public Header
{
public:
    DefaultHeader();
};

#endif // HEADER_H
