#ifndef RECT_H
#define RECT_H

#include "object.h"

#include <QRectF>

class Rect : public Object, public QRectF
{

    // In millimeters
    float linewidth = 0.5;
    friend class GRect;

public:
    Rect(qreal x, qreal y, qreal width, qreal height);

};

#endif // RECT_H
