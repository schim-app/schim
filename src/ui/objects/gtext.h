#ifndef GTEXT_H
#define GTEXT_H

#include "gobject.h"

#include "model/text.h"

class GText : public GObject
{
public:
    GText(Text *obj = new Text());

    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    void reload();

    Text *get();
    const Text *get() const;

private:
    float getWidth() const;
};

#endif // GTEXT_H
