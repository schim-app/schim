#ifndef GRECT_H
#define GRECT_H

#include "gobject.h"
#include "model/rect.h"

#include <QGraphicsRectItem>


class GRect : public GObject
{
public:
    GRect(Rect *obj = new Rect);

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void reload() override;
    void apply() override;

    void showHandles(bool show = true) override;
    void handleChanged(GObjectHandle *handle) override;

    Rect *get();
    const Rect *get() const;
};

#endif // GRECT_H
