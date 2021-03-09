#ifndef GCOMPOSITEOBJECT_H
#define GCOMPOSITEOBJECT_H

#include "gobject.h"

#include "model/compositeobject.h"

class GCompositeObject : public GObject
{
public:
    // CONSTRUCTORS
    GCompositeObject(CompositeObject *obj);

    // GETTERS
    CompositeObject *get();
    const CompositeObject *get() const;

    // SETTERS
    /**
     * @copydoc GObject
     *
     * For correct behavior, call this method only after the object has been
     * added to a scene. This will ensure that it is applied to all children of
     * the object.
     */
    void setCosmetic(bool cosmetic) override;

    // OVERRIDE QGraphicsObject
    QPainterPath shape() const override;

    // MISCELLANEOUS
    void apply() override;
    void reload() override;

protected:
    // EVENTS
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant &value) override;
};

#endif // GCOMPOSITEOBJECT_H
