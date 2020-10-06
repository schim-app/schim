#ifndef GOBJECT_H
#define GOBJECT_H

#include "model/object.h"
#include "model/line.h"

#include <QGraphicsItem>

class GObject : public QGraphicsItem
{
    Object *obj;
public:
    GObject();
    GObject(Object *obj);

    const Object *getObject() const;
    Object *getObject();



    /**
     * Dynamically allocate a GObject, where the actual type depends on the type
     * of \param obj.
     */
    static GObject *assign(Object *obj);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GOBJECT_H
