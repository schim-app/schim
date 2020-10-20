#ifndef HEADER_H
#define HEADER_H

#include "object.h"
#include "rect.h"
#include "compositeobject.h"

/**
 * Class representing a header for a sheet.
 * It is a specialization of CompositeObject and its content is user
 * configurable. For example, the user can enable/disable column and row markers
 * and set their pattern.
 */
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

    QRectF getContentArea() const;

protected:
    /**
     * Clear all objects that are configurable (display rows, columns, etc.) from the list of children.
     * The objects are also deleted. Objects that are not configurable are untouched.
     */
    virtual void clear();

private:
    QRectF contentArea{5, 5, 287, 175};
};

#endif // HEADER_H
