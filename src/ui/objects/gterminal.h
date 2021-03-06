#ifndef GTERMINAL_H
#define GTERMINAL_H

#include "gobject.h"

#include "model/terminal.h"

class GTerminal : public GObject
{
public:
    explicit GTerminal(Terminal *obj);

    // GETTERS
    Terminal *get();
    const Terminal *get() const;

    void reloadFromModel() override;
    void applyToModel() override;

protected:
    // QGraphicsItem INTERFACE
    QRectF boundingRect() const override;
    void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override;
    QVariant itemChange(GraphicsItemChange, const QVariant &) override;
};

class GConnectionSuggester : public QGraphicsLineItem
{
public:
    GConnectionSuggester(const Terminal::Prong &p1, const Terminal::Prong &p2);
};

#endif // GTERMINAL_H
