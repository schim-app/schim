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
    GLine(Line *obj);

    Line *get();
    const Line *get() const;

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void reload() override;
    void apply() override;

private:

    void handleChanged(GObjectHandle *handle) override;

};

#endif // GLINE_H
