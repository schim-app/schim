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
private:
    // The position in the sheet of the upper-left corner of the object

public:

    Object() = default;
    /** @brief Copy constructor */
    Object(const Object &object) = default;

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
    Sheet *getSheet() const;
    CompositeObject *getParent() const;

    /**
     * @brief Set the position of the object in the sheet.
     */
    virtual void setPos(const QPointF &pos);
    void setSheet(Sheet *sheet);
    void setParent(CompositeObject *parent);

    virtual void setProperty(const QString &name, const QString &value);
    virtual QString getProperty(const QString &name) const;

    virtual Object *clone() const = 0;

protected:
    CompositeObject *parent{};
    Sheet *sheet{};

    friend class Sheet;
    friend class GObject;
    friend class CompositeObject;

};

#endif // ELEMENT_H
