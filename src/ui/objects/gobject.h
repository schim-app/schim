#ifndef GOBJECT_H
#define GOBJECT_H

#include "model/object.h"
#include "model/line.h"

#include <QGraphicsItem>

class GObject : public QGraphicsItem
{
protected:
    /** The object that is being wrapped */
    Object *obj;
public:
    GObject(Object *obj);

    /**
     * @brief Return the object that is being wrapped by this class
     *
     * @note Derived classes should change the return type of this method
     * to match the type of the object that is wrapped by them.
     */
    Object *get();
    const Object *get() const;
    /** @brief Return whether the mouse is over this object. */
    bool isHovered() const;
    /** @brief Same as base implementation but the return value is cast to GObject*. */
    GObject *parentItem();

    /**
     * Return a dynamically allocated GObject wrapping the specified object.
     *
     * The actual type of the returned object is determined
     * based on the actual type of `obj`. For example, if `obj` is
     * of type Line*, then a GLine* will be returned.
     *
     * @param obj Object to wrap.
     */
    static GObject *assign(Object *obj);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    bool hovered = false;
};

#endif // GOBJECT_H
