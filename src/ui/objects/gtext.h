#ifndef GTEXT_H
#define GTEXT_H

#include "gobject.h"

#include "model/text.h"

//TODO remove
#include "ui/mainwindow.h"

class GDisplayText;

class GText : public GObject
{
    Q_OBJECT
public:
    GText(Text *obj = new Text);
    ~GText();

    // GETTERS
    Text *get();
    const Text *get() const;
    /** Cast the scene to a `SheetScene*`. */
    SheetScene *scene();

    // OVERRIDEN QGraphicsItem METHODS
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

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
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    /** On double left click: enable editing, change to IBeam cursor and hide cursor guides. */
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    /** Hide cursor guides. */
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    /** Show cursor guides. */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    // FOR EDITING THE OBJECT

    void reload() override;
    void apply() override;
    void showContextMenu();
    void setEditMode(bool edit);

private slots:
    void edit();
    void onFocusOut();

private:
    GDisplayText *displayItem;
    bool editMode = false;
};

class GDisplayText : public QGraphicsTextItem
{
    Q_OBJECT
public:
    // EVENTS
    /** Disable editing, reset mouse cursor to default, and clear text selection. */
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // CUSTOM METHODS
    /** Cast the scene to a `SheetScene*`. */
    SheetScene *scene();
    void setParentItem(GText *parent);
    GText *parentItem();

signals:
    void focusOut();

private:
    bool editMode = false;
    friend class GText;
};

#endif // GTEXT_H
