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

/**
 * @brief The base class of all objects represented in a graphical scene.
 *
 * This class is conceptually an abstract one - it shouldn't be
 * used on its own.
 */
class GObject : public QGraphicsItem
{
public:
    /**
     * @brief Construct a wrapper around `obj`.
     *
     * The object is movable, selectable and sends geometry changes by default.
     * Hover events are accepted.
     */
    GObject(Object *obj);

    /** Destroy any handles that are still active. */
    virtual ~GObject();

    // GETTERS

    /**
     * @brief Return the object that is being wrapped by this class.
     *
     * @note Derived classes should change the return type of this method
     * to match the type of the object that is wrapped by them.
     */
    Object *get();
    const Object *get() const;
    /** @brief Return whether the mouse is over this object. */
    bool isHovered() const;

    // OVERRIDEN METHODS

    /** The default implementation returns childrenBoundingRect(). */
    QRectF boundingRect() const override;
    /**
     * This implementation applies aesthetic modifications when the
     * object is hovered or selected.
     *
     * @note Derived classes should call this version at the beginning
     * of their implementation to apply the aesthetic changes.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    /** Return the parent item cast to a `GObject*`. */
    GObject *parentItem();
    /** Return the scene cast to a `SheetScene*`. */
    SheetScene *scene();

    // FOR EDITING THE OBJECT

    /**
     * @brief Update the graphical representation to match the object from the model.
     *
     * @note The base implementation does nothing and should be implemented
     * in derived classes.
     */
    virtual void reload();
    /**
     * @brief Apply changes to the underlying model object.
     *
     * @note The base implementation does nothing and should be implemented
     * in derived classes.
     */
    virtual void apply();
    /**
     * @brief Display/hide the handles for this item.
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
    /**
     * @brief Called when a handle's position has changed.
     *
     * The base implementation does nothing.
     * @param handle The handle that has been modified.
     */
    virtual void handleChanged(GObjectHandle *handle);

    // STATIC

    /**
     * Return a dynamically allocated GObject wrapping the specified object.
     *
     * The actual type of the returned object is determined
     * based on the actual type of `obj`. For example, if `obj` is
     * of type Line*, then a GLine* will be returned.
     */
    static GObject *assign(Object *obj);

protected:
    // EVENTS

    /**
     * If the item is being dragged (left button is down),
     * snap the cursor guides to the grid.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    /** Enable the bool `hovered` so that it can be used by paint. */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    /** Disable the bool `hovered` so that it can be used by paint. */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    /**
     * Process item changes that should behave uniformly across different
     * object types. All derived objects should call this version in their implementation.
     *
     * The following changes are processed:
     * - `ItemZValueHasChanged`: display the handles above this object.
     * - `ItemSelectedHasChanged`: show the handles.
     * - `ItemSceneHasChanged`: TODO
     * - `ItemPositionChange`: apply snap-to-grid before the position actually changes.
     * - `ItemPositionHasChanged`: apply position change to the underlying model object.
     */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    // HELPER METHODS

    /** Assign a z-value to the handles that is higher than that of the object. */
    void moveHandlesAbove();

protected:
    // PROTECTED ATTRIBUTES

    /** The object that is being wrapped */
    Object *obj;
    /** Dynamically allocated list of handles */
    QList<GObjectHandle*> *handles{};

private:
    // PRIVATE ATTRIBUTES

    bool hovered = false;

    // FRIENDS

    friend class GObjectHandle;
};

/**
 * @brief The base class for object handles.
 *
 * A handle is an item that allows the user to manipulate the
 * object in some way, e.g. to resize, rotate, etc.
 */
class GObjectHandle : public QGraphicsRectItem
{

public:
    /**
     * @brief Construct a handle associated with the specified GObject.
     *
     * `obj` is assigned as the parent of the handle in the scene.
     * The handle is configured to send geometry changes.
     */
    GObjectHandle(GObject *obj);

    /** Return the scene cast to a SheetScene*. */
    SheetScene *scene();

protected:
    // EVENTS

    /** Used to notify `obj` of any changes to this handle */
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;
    /**
     * @brief Initiate dragging when the left button is pressed.
     *
     * The handle accepts the event, which means that it will not be further
     * processed by the scene.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /**
     * @brief Move the handle when the mouse is dragged over it with the left button down.
     *
     * This method is used because the selection flag is disabled due to practical reasons.
     * The handle is moved using moveBy, whereby snap-to-grid is applied
     * if appropriate. The event is accepted.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // PRIVATE ATTRIBUTES

    GObject *obj;
    bool selected = false;
    QPointF _dragStartPos;
};

#endif // GOBJECT_H
