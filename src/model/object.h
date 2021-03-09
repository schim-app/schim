#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPointF>
#include <QString>

class CompositeObject;
class Sheet;

/**
 * @brief The **abstract** base class for all objects in a sheet.
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
public:

    // CONSTRUCTORS
    /**
     * @brief This destructor does nothing.
     *
     * It is declared virtual in order to be properly used by derived classes.
     */
    virtual ~Object() = default;
    virtual Object *clone() const = 0;

    // GETTERS
    /// @brief Return the position of the object in the sheet.
    virtual QPointF getPos() const = 0;
    Sheet *getSheet() const;
    CompositeObject *getParent() const;
    virtual QString getProperty(const QString &name) const;

    // SETTERS
    /// @brief Set the position of the object in the sheet.
    virtual void setPos(const QPointF &pos) = 0;
    void setSheet(Sheet *sheet);
    void setParent(CompositeObject *parent);
    virtual void setProperty(const QString &name, const QString &value);

    // OPERATORS
    virtual bool operator==(const Object &obj) const;
    virtual bool operator!=(const Object &obj) const;

protected:
    // ATTRIBUTES
    CompositeObject *parent{};
    Sheet *sheet{};
};

#endif // ELEMENT_H
