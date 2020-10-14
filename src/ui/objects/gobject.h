#ifndef GOBJECT_H
#define GOBJECT_H

#include "model/object.h"
#include "model/line.h"

#include <QGraphicsItem>

class GObject : public QGraphicsItem
{
protected:
    Object *obj;
public:
    GObject(Object *obj);

    const Object *get() const;
    Object *get();
    bool isHovered() const;
    GObject *parentItem();

    /**
     * Dynamically allocate a GObject, where the actual type depends on the type
     * of \param obj.
     */
    static GObject *assign(Object *obj);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool hovered = false;
};

#endif // GOBJECT_H
