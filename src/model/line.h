#ifndef LINE_H
#define LINE_H

#include "object.h"

#include <QLineF>

/**
 * Class that represents a line object.
 *
 * It inherits QLineF so we don't have to reimplement convenience
 * functions that QLineF already provides.
 */
class Line : public Object, public QLineF
{
    // Everything is in millimeters
    float linewidth = 0.5;
public:
    Line();
    Line(qreal x1, qreal y1, qreal x2, qreal y2);

    float getLinewidth() const;
    QPointF getPos() const override;

    void setLinewidth(float w);
    void setPos(const QPointF &pos) override;

    friend class GLine;
};

#endif // LINE_H
