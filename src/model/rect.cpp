#include "rect.h"

Rect::Rect(qreal x, qreal y, qreal width, qreal height)
    : Object(), QRectF(x, y, width, height) { }
