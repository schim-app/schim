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
    /**
     * @brief Default constructor.
     */
    Header() = default;
    /**
     * @brief Move constructor
     *
     * Take all children from `obj`. Consequently, `obj` is emptied and this
     * object takes ownership of its children.
     */
    Header(CompositeObject &&obj);
    /**
     * @brief Copy constructor.
     */
    Header(const Header &obj);
    Header *clone() const;

    // GETTERS
    QRectF getContentArea() const;

    // SETTERS
    void setContentArea(const QRectF &rect);

    // STATIC
    /**
     * @brief Steal everything from `obj` and delete it.
     *
     * This creates a new component containing all constituents from `obj`, and
     * then destroys `obj`.
     */
    static Header *absorb(CompositeObject *obj);

    // OPERATORS
    bool operator==(const Header &obj) const;
    bool operator!=(const Header &obj) const;

private:
    /// The area where the schematic content should be.
    QRectF contentArea{0, 0, 297, 210};
    /// Both of these strings should be bash-readable arrays.
    QString columnMarkers = "{0..9}", rowMarkers = "{A..F}";
    /// Display options.
    bool displayColumns = true, displayRows = true;
};

#endif // HEADER_H
