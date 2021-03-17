/// @file gline.h
#ifndef GLINE_H
#define GLINE_H

#include "model/object.h"
#include "model/line.h"
#include "ui/objects/gobject.h"

#include <QGraphicsLineItem>
#include <QVariant>

/**
 * @brief Graphical line object that wraps a `Line`.
 */
class GLine : public GObject
{
public:
    // CONSTRUCTORS
    explicit GLine(Line *obj = new Line);

    // GETTERS
    Line *get() override;
    const Line *get() const override;

    // OBJECT EDITING
    void reloadFromModel() override;
    void applyToModel() override;
    void handleChanged(GObjectHandle *handle) override;
    void showHandles(bool show = true) override;

    // OVERRIDE QGraphicsItem
    QRectF boundingRect() const override;
protected:
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // GLINE_H
