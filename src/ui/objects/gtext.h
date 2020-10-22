#ifndef GTEXT_H
#define GTEXT_H

#include "gobject.h"

#include "model/text.h"

class GText : public GObject, public QGraphicsTextItem
{
public:
    GText(Text *obj = new Text);

    // GETTERS

    Text *get();
    const Text *get() const;

    // OVERRIDEN QGraphicsItem METHODS

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    /** Cast the scene to a `SheetScene*`. */
    SheetScene *scene();

    // EVENTS

    /** Use `GObject`'s implementation */
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    /**
     * In editing mode, let QGraphicsTextItem process the event.
     * Otherwise, let GObject process the event, making it behave
     * like any other item.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    /** @see GText::mousePressEvent */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    /** @see GText::mousePressEvent */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    /** On double left click: enable editing, change to IBeam cursor and hide cursor guides. */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    /** Hide cursor guides. */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    /** Show cursor guides. */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    /**
     * In editing mode, update the scene to show changes (which is not done by default).
     * Also, accept the event to prevent keyboard input from being processed as a shortcut.
     * Out of editing mode, let GObject process the event.
     */
    void keyPressEvent(QKeyEvent *event) override;
    /** In editing mode, update the model object based on the text input by calling apply. */
    void keyReleaseEvent(QKeyEvent *event) override;

    /** Disable editing, reset mouse cursor to default, and clear text selection. */
    void focusOutEvent(QFocusEvent *event) override;

    // FOR EDITING THE OBJECT

    void reload() override;
    void apply() override;

};

#endif // GTEXT_H
