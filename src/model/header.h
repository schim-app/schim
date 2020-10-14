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

    /** Update the graphical model to match the configuration. */
    virtual void update();
    /** Initialize the graphical model with objects that are non-configurable */
    virtual void init();

protected:
    /**
     * Clear all objects that are configurable (display rows, columns, etc.) from the list of children.
     * The objects are also deleted. Objects that are not configurable are untouched.
     */
    virtual void clear();
};

class DefaultHeader : public Header
{
public:

    DefaultHeader();

    virtual void update() override;
    virtual void init() override;

protected:
    virtual void clear() override;
};

#endif // HEADER_H
