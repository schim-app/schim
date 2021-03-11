#ifndef RECT_H
#define RECT_H

#include "object.h"

#include <QRectF>

/**
 * @brief A visual rectangle object.
 *
 * Inherits QRectF so we don't have to reimplement convenience functions that
 * QRectF already provides.
 */
class Rect : public Object, public QRectF
{
public:
    // CONSTRUCTORS
    /**
     * @brief Construct a null rectangle.
     */
    Rect();
    /**
     * @brief Usual way to construct a rectangle.
     */
    Rect(qreal x, qreal y, qreal width, qreal height);
    /**
     * @brief Copy constructor
     */
    Rect(const Rect &rect);
    Object *clone() const override;

    // GETTERS
    QPointF getPos() const override;
    float getLinewidth() const;
    QString getProperty(const QString &name) const override;

    // SETTERS
    void setPos(const QPointF &pos) override;
    void setLinewidth(float lw);
    void setProperty(const QString &name, const QString &value) override;

    // OPERATORS
    bool operator==(const Rect &obj) const;
    bool operator!=(const Rect &obj) const;

private:
    // ATTRIBUTES
    /// In millimeters
    float linewidth = 0.5;
};

#endif // RECT_H
