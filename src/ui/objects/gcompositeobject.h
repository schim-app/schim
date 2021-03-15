#ifndef GCOMPOSITEOBJECT_H
#define GCOMPOSITEOBJECT_H

#include "gobject.h"

#include "model/compositeobject.h"

/**
 * @brief A graphical object that wraps a `CompositeObject`.
 */
class GCompositeObject : public GObject
{
public:
    // CONSTRUCTORS
    explicit GCompositeObject(CompositeObject *obj);

    // GETTERS
    virtual CompositeObject *get() override;
    virtual const CompositeObject *get() const override;

    // SETTERS
    /**
     * @copydoc GObject
     *
     * For correct behavior, call this method only after the object has been
     * added to a scene. This will ensure that it is applied to all children of
     * the object.
     */
    void setCosmetic(bool cosmetic) override;

    // MISCELLANEOUS
    void applyToModel() override;
    void reloadFromModel() override;

protected:
    // EVENTS
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;

    // OVERRIDE QGraphicsObject
    virtual QPainterPath shape() const override;
    virtual QRectF boundingRect() const override;
};

#endif // GCOMPOSITEOBJECT_H
