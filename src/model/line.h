#ifndef LINE_H
#define LINE_H

#include "object.h"

#include <QPointF>

class Line : public Object
{
    QPointF start, end; // in millimeters
    float linewidth = 0.5; // in millimeters
public:
    Line(const QPointF &start, const QPointF &end);
};

#endif // LINE_H
