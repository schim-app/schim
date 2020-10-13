#ifndef LINE_H
#define LINE_H

#include "object.h"

#include <QLineF>

class Line : public Object, public QLineF
{
    // Everything is in millimeters
    float linewidth = 0.5;
public:
    Line(qreal x1, qreal y1, qreal x2, qreal y2);

    friend class GLine;
};

#endif // LINE_H
