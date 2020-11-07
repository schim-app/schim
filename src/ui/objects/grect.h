#ifndef GRECT_H
#define GRECT_H

#include "gobject.h"
#include "model/rect.h"

#include <QGraphicsRectItem>


/**
 * @brief Graphical representation of a rectangle.
 */
class GRect : public GObject
{
public:
    GRect(Rect *obj = new Rect);

public:
    // GETTERS

    Rect *get();
    const Rect *get() const;

    // OVERRIDDEN QGraphicsItem METHODS

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    // FOR EDITING THE OBJECT

    /** @copybrief GObject::reload */
    void reload() override;
    /** @copybrief GObject::apply */
    void apply() override;
    /** @copybrief GObject::showHandles */
    void showHandles(bool show = true) override;
    /** @copybrief GObject::handleChanged */
    void handleChanged(GObjectHandle *handle) override;

};

#endif // GRECT_H
