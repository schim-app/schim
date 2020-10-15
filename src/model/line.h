#ifndef LINE_H
#define LINE_H

#include "object.h"

#include <QLineF>

/**
 * Class that represents a line object.
 * It inherits QLineF as well so we don't have to implement convenience
 * functions that are already part of QLineF.
 */
class Line : public Object, public QLineF
{
    // Everything is in millimeters
    float linewidth = 0.5;
public:
    Line(qreal x1, qreal y1, qreal x2, qreal y2);

    friend class GLine;
};

#endif // LINE_H
