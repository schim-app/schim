#ifndef GTERMINAL_H
#define GTERMINAL_H

#include "gobject.h"

#include "model/terminal.h"

class GTerminal : public GObject
{
public:
    GTerminal(Terminal *obj);

    // GETTERS
    Terminal *get();
    const Terminal *get() const;

    void reload();
    void apply();

protected:
    // QGraphicsItem INTERFACE
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;

    // QGraphicsItem interface
    QVariant itemChange(GraphicsItemChange, const QVariant &) override;
};

#endif // GTERMINAL_H
