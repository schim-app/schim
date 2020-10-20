#ifndef GOBJECT_H
#define GOBJECT_H

#include "model/object.h"
#include "model/line.h"
#include "ui/sheetscene.h"

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

class GObjectHandle;

class GObject : public QGraphicsItem
{
protected:
    /** The object that is being wrapped */
    Object *obj;
    /** Dynamically allocated list of handles */
    QList<GObjectHandle*> *handles{};

public:
    GObject(Object *obj);
    virtual ~GObject();

    // Getters

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
    /** @brief Same as base implementation but the return value is cast to GObject*. */
    GObject *parentItem();
    SheetScene *scene();


    /**
     * @brief Update the graphical representation to match the object from the model.
     */
    virtual void reload();
    /**
     * @brief Apply changes to the underlying model object.
     */
    virtual void apply();

    /**
     * @brief Display the move/resize/etc. handles for this item.
     *
     * The list of handles is dynamically allocated and it exists
     * only while the handles are visible. This way, the handles
     * do not take up memory when they are not used.
     *
     * @note The base version of this method implements only the
     * deletion of handles, i.e. when `show = true`.
     * Each subclass should implement this this method separately,
     * optionally calling the superclass method **but only as the last statement**.
     */
    virtual void showHandles(bool show = true);
    virtual void handleChanged(GObjectHandle *handle);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    void moveHandlesAbove();

private:
    bool hovered = false;

    friend class GObjectHandle;
};

class GObjectHandle : public QGraphicsRectItem
{

public:
    GObjectHandle(GObject *obj);

    SheetScene *scene();

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    GObject *obj;
    bool selected = false;
    QPointF _dragStartPos;

};

#endif // GOBJECT_H
