#include "line.h"

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2)
    : QLineF(x1, y1, x2, y2) {}

float Line::getWidth() const
{
    return linewidth;
}

QPointF Line::getPos() const
{
    return p1();
}

void Line::setWidth(float w)
{
    linewidth = w;
}
