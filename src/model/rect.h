#ifndef RECT_H
#define RECT_H

#include "object.h"

#include <QRectF>

/**
 * @brief Class that represents a Rectangle object.
 *
 * It inherits QRectF so we don't have to reimplement convenience
 * functions that QRectF already provides.
 */
class Rect : public Object, public QRectF
{

    // In millimeters
    float linewidth = 0.5;

public:
    Rect();
    Rect(qreal x, qreal y, qreal width, qreal height);
    /** @brief Copy constructor */
    Rect(const Rect &rect);

    QPointF getPos() const override;
    float getLinewidth() const;

    void setPos(const QPointF &pos) override;
    void setLinewidth(float lw);

    void setProperty(const QString &name, const QString &value) override;
    QString getProperty(const QString &name) const override;

    Object *clone() const override;

    friend class GRect;
};

#endif // RECT_H
