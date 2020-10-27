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
};

#endif // GCOMPOSITEOBJECT_H
