#ifndef GRECT_H
#define GRECT_H

#include "gobject.h"
#include "model/rect.h"

#include <QGraphicsRectItem>


class GRect : public GObject
{
public:
    GRect(Rect *obj);

public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

    Rect *get();
    const Rect *get() const;
};

#endif // GRECT_H
