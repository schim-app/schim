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
    /** @brief Copy constructor */
    Line(const Line &line);
    Object *clone() const override;

    float getLinewidth() const;
    QPointF getPos() const override;

    void setLinewidth(float w);
    void setPos(const QPointF &pos) override;

    // PROPERTIES
    void setProperty(const QString &name, const QString &value) override;
    QString getProperty(const QString &name) const override;

    // OPERATORS
    bool operator==(const Line &obj) const;
    bool operator!=(const Line &obj) const;

    friend class GLine;
};

#endif // LINE_H
