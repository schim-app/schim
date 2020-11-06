#include "rect.h"

Rect::Rect() { }

Rect::Rect(qreal x, qreal y, qreal width, qreal height)
    : Object(), QRectF(x, y, width, height) { }

Rect::Rect(const Rect &rect)
    : Object(), QRectF(rect)
{
    linewidth = rect.linewidth;
}

QPointF Rect::getPos() const
{
    return topLeft();
}

void Rect::setPos(const QPointF &pos)
{
    setRect(pos.x(), pos.y(), width(), height());
}

Object *Rect::clone() const
{
    return new Rect(*this);
}
