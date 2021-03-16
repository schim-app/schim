#ifndef COMPOSITEOBJECT_H
#define COMPOSITEOBJECT_H

#include "model/object.h"
#include "model/entity.h"
#include "model/variable.h"

/**
 * @brief An object that consists of other child objects.
 *
 * This is the most general representation of an object. Destruction
 * of its child objects should always be performed in the destructor.
 * This class inherits QList<Object*> so we don't have to
 * reimplement the convenience functions that QList already provides.
 */
class CompositeObject : public Object, public Entity
{

public:
    // CONSTRUCTORS
    CompositeObject();
    /// @brief Copy constructor
    CompositeObject(const CompositeObject &obj);
    virtual ~CompositeObject();
    Object *clone() const override;

    // GETTERS
    QPointF getPos() const override;
    QList<Object *> &getConstituents();
    const QList<Object *> &getConstituents() const;

    // SETTERS
    void setPos(const QPointF &pos) override;
    void add(Object *obj);
    void add(const QList<Object*> &list);
    /**
     * @brief Remove `obj` from the object.
     *
     * If this is an instance of `CompositeObject`, then `obj` is removed from
     * its list of constituents. The behavior can vary in subclasses.
     * @see For an example behavior in subclasses, see `Component::remove`.
     */
    virtual void remove(Object *obj);

    // OPERATORS
    bool operator==(const CompositeObject &obj) const;
    bool operator!=(const CompositeObject &obj) const;

protected:
    QList<Object *> constituents;
    // TODO determine appropriate container
    QPointF pos{};

};

#endif // COMPOSITEOBJECT_H
