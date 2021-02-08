#include "rect.h"

Rect::Rect() { }

Rect::Rect(qreal x, qreal y, qreal width, qreal height)
    : Object(), QRectF(x, y, width, height) { }

Rect::Rect(const Rect &rect)
    : Object(), QRectF(rect)
{
    linewidth = rect.linewidth;
}

QPointF Rect::getPos() const
{
    return topLeft();
}

float Rect::getLinewidth() const
{
    return linewidth;
}

void Rect::setPos(const QPointF &pos)
{
    setRect(pos.x(), pos.y(), width(), height());
}

void Rect::setLinewidth(float lw)
{
    linewidth = lw;
}

void Rect::setProperty(const QString &name, const QString &value)
{
    float val;
    bool conversion_ok;

#define set_float_property(NAME, COMMAND) \
    if (name == NAME) { \
        val = value.toFloat(&conversion_ok); \
        if (!conversion_ok) \
            throw std::logic_error("Rectangle property \"" + name.toStdString() + "\" is of invalid format"); \
        COMMAND(val); \
        return; \
    }

    set_float_property("x", setLeft);
    set_float_property("y", setTop);
    set_float_property("w", setWidth);
    set_float_property("h", setHeight);

    set_float_property("linewidth", setLinewidth);
    set_float_property("lw", setLinewidth);;
    // else
    throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for rectangle");
}

QString Rect::getProperty(const QString &name) const
{
    if (name == "x") return QString::number(left());
    if (name == "y") return QString::number(right());
    if (name == "w") return QString::number(width());
    if (name == "h") return QString::number(height());
    if (name == "linewidth" || name == "lw") return QString::number(linewidth);
    // else
    throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for rectangle");
}

Object *Rect::clone() const
{
    return new Rect(*this);
}
