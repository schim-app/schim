/// @file gtext.h
#ifndef GTEXT_H
#define GTEXT_H

#include "gobject.h"

#include "model/text.h"

class GDisplayText;

/**
 * @brief A graphical text object that wraps a `Text`.
 *
 * The `GText` has a child item of type `GDisplayText` that displays the text
 * to the user and handles text editing.
 *
 * `GText` is responsible for:
 *
 * - synchronization with the model
 * - manipulation of `GDisplayText` flags to enter/leave edit mode
 * - any interaction that doesn't involve text editing, e.g object selection,
 * displaying context menus, etc.
 */
class GText : public GObject
{
    Q_OBJECT
public:
    explicit GText(Text *obj = new Text);
    ~GText();

    // GETTERS
    Text *get() override;
    const Text *get() const override;
    bool isInEditMode() const;

    // OBJECT EDITING
    void setEditMode(bool edit);
    void reloadFromModel() override;
    void applyToModel() override;

signals:
    void focusOut();

protected:
    // EVENTS
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    /**
     * On double left click: enable editing, change to IBeam cursor and hide
     * cursor guides.
     */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    /** Hide cursor guides. */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    /** Show cursor guides. */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

private slots:
    void onContextEdit();
    void onFocusOut();

private:
    // OVERRIDE QGraphicsItem
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    // ATTRIBUTES
    GDisplayText *displayItem;
};

/**
 * @brief A helper item for `GText` that displays its textual content.
 */
class GDisplayText : public QGraphicsTextItem
{
    Q_OBJECT
protected:
    // EVENTS
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

signals:
    /**
     * @brief Emitted when the item loses focus.
     *
     * The `GText` parent of this item should handle this event.
     */
    void focusOut();

public:
    // OVERRIDE QGraphicsItem
    void setParentItem(GText *parent);
    GText *parentItem();
protected:
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
    friend class GText;
};

#endif // GTEXT_H
