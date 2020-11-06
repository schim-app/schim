#include "line.h"

Line::Line() { }

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2)
    : QLineF(x1, y1, x2, y2) {}

Line::Line(const Line &line)
    : Object(line), QLineF(line)
{
    linewidth = line.linewidth;
}

float Line::getLinewidth() const
{
    return linewidth;
}

QPointF Line::getPos() const
{
    return p1();
}

void Line::setLinewidth(float w)
{
    linewidth = w;
}

void Line::setPos(const QPointF &pos)
{
    setP1(pos);
}

Object *Line::clone() const
{
    return new Line(*this);
}
