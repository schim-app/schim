#ifndef RECT_H
#define RECT_H

#include "object.h"

#include <QPointF>

class Rect : public Object
{
    QPointF pivot;
    float width, height;

    friend class GRect;

public:
    Rect(QPointF pivot, float width, float height);

};

#endif // RECT_H
