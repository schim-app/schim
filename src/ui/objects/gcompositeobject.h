#ifndef GCOMPOSITEOBJECT_H
#define GCOMPOSITEOBJECT_H

#include "gobject.h"

#include "model/compositeobject.h"

class GCompositeObject : public GObject
{
public:
    GCompositeObject(CompositeObject *obj);

    CompositeObject *get();
    const CompositeObject *get() const;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    QPainterPath shape() const override;

    /**
     * @copydoc GObject
     *
     * For correct behavior, call this method only after the object has been
     * added to a scene. This will ensure that it is applied to all children
     * of the object
     */
    void setCosmetic(bool cosmetic) override;

    void apply() override;
    void reload() override;

};

#endif // GCOMPOSITEOBJECT_H
