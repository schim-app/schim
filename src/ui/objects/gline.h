#ifndef GLINE_H
#define GLINE_H

#include "model/object.h"
#include "model/line.h"
#include "ui/objects/gobject.h"

#include <QGraphicsLineItem>

class GLine : public GObject
{
public:
    GLine(Line *obj);

    Line *get();
    const Line *get() const;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GLINE_H
