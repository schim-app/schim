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
    // CONSTRUCTORS
    GRect(Rect *obj = new Rect);

    // GETTERS
    Rect *get();
    const Rect *get() const;

    // OVERRIDDE QGraphicsItem
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;

    // OBJECT EDITING
    /** @copybrief GObject::reload */
    void reloadFromModel() override;
    /** @copybrief GObject::apply */
    void applyToModel() override;
    /** @copybrief GObject::showHandles */
    void showHandles(bool show = true) override;
    /** @copybrief GObject::handleChanged */
    void handleChanged(GObjectHandle *handle) override;

};

#endif // GRECT_H
