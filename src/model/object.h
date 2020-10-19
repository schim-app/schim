#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPointF>

/**
 * @brief The base class for all objects in a sheet.
 *
 * Everything that has a visual representation in a sheet is derived from this
 * class.
 *
 * This is a model class, which is distinct from a graphical class. A
 * graphical class represents a wrapper around model classes so that they can be
 * represented in a QGraphicsScene. Model classes are independent of any user
 * interface.
 */
class Object
{
private:
    // The position in the sheet of the upper-left corner of the object

protected:

public:

    /**
     * @brief This destructor does nothing.
     *
     * It is declared virtual in order to be properly used
     * by derived classes.
     */
    virtual ~Object() = default;

    /**
     * @brief Return the position of the object in the sheet.
     */
    virtual QPointF getPos() const;

    /**
     * @brief Set the position of the object in the sheet.
     */
    virtual void setPos(const QPointF &pos);

    friend class GObject;

};

#endif // ELEMENT_H
