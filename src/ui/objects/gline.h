#ifndef GLINE_H
#define GLINE_H

#include "model/object.h"
#include "model/line.h"
#include "ui/objects/gobject.h"

#include <QGraphicsLineItem>

class GLine : public QGraphicsLineItem, public GObject
{
public:
    GLine(Line *obj);

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // GLINE_H
