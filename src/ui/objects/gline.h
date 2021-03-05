#ifndef GLINE_H
#define GLINE_H

#include "model/object.h"
#include "model/line.h"
#include "ui/objects/gobject.h"

#include <QGraphicsLineItem>
#include <QVariant>

class GLine : public GObject
{
public:
    // CONSTRUCTORS
    GLine(Line *obj = new Line);

    // GETTERS
    Line *get();
    const Line *get() const;

    // OVERRIDE QGraphicsObject
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    // OBJECT EDITING
    void reload() override;
    void apply() override;
    void handleChanged(GObjectHandle *handle) override;
    void showHandles(bool show = true) override;
};

#endif // GLINE_H
