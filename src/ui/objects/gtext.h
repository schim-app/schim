#ifndef GTEXT_H
#define GTEXT_H

#include "gobject.h"

#include "model/text.h"

class GText : public GObject, public QGraphicsTextItem
{
public:
    GText(Text *obj = new Text());

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void focusOutEvent(QFocusEvent *event) override;

    void reload() override;

    Text *get();
    const Text *get() const;

};

#endif // GTEXT_H
