#ifndef HEADER_H
#define HEADER_H

#include "object.h"
#include "rect.h"
#include "compositeobject.h"

/**
 * @brief Class representing a header for a sheet.
 *
 * It is a specialization of `CompositeObject` and its content is
 * user-configurable.  For example, the user can enable/disable column and row
 * markers and set the pattern that generates them.
 */
class Header : public CompositeObject
{

public:
    // CONSTRUCTORS
    /// @brief Default constructor.
    Header() = default;
    /**
     * @brief Move constructor
     *
     * Take all children from `obj`. Consequently, `obj` is emptied and this
     * object becomes responsible for the destruction of the children.
     */
    Header(CompositeObject &&obj);
    /// @brief Steal everything from `obj` and delete it.
    Header(CompositeObject *obj);
    /// @brief Copy constructor.
    Header(const Header &obj);
    Header *clone() const;

    // GETTERS
    QRectF getContentArea() const;

    // SETTERS
    void setContentArea(const QRectF &rect);

    // OPERATORS
    bool operator==(const Header &obj) const;
    bool operator!=(const Header &obj) const;

private:
    /// @brief The area where the schematic content should be.
    QRectF contentArea{0, 0, 297, 210};
    /// Both of these strings represent bash-readable arrays.
    QString columnMarkers = "{0..9}", rowMarkers = "{A..F}";
    /// Display options.
    bool displayColumns = true, displayRows = true;
};

#endif // HEADER_H
