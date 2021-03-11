#ifndef LINE_H
#define LINE_H

#include "object.h"

#include <QLineF>

/**
 * @brief A visual line object.
 *
 * A line is represented by two points. We take its position to be the first of
 * those points.
 *
 * @note `Line` is derived from `QLineF`, so you can use all its base functions.
 */
class Line : public Object, public QLineF
{
public:
    // CONSTRUCTORS
    /**
     * @brief Construct a null line.
     */
    Line();
    /**
     * @brief Usual way to construct a line.
     */
    Line(qreal x1, qreal y1, qreal x2, qreal y2);
    /**
     * @brief Copy constructor
     */
    Line(const Line &line);
    Object *clone() const override;

    // GETTERS
    float getLinewidth() const;
    QPointF getPos() const override;
    QString getProperty(const QString &name) const override;

    // SETTERS
    void setLinewidth(float w);
    void setPos(const QPointF &pos) override;
    void setProperty(const QString &name, const QString &value) override;

    // OPERATORS
    bool operator==(const Line &obj) const;
    bool operator!=(const Line &obj) const;

private:
    // ATTRIBUTES
    /// Everything is in millimeters
    float linewidth = 0.5;
};

#endif // LINE_H
