#include "line.h"

// CONSTRUCTORS

Line::Line() { }

Line::Line(qreal x1, qreal y1, qreal x2, qreal y2)
    : QLineF(x1, y1, x2, y2) {}

Line::Line(const Line &line)
    : Object(line), QLineF(line), linewidth(line.linewidth) {}

Object *Line::clone() const
{
    return new Line(*this);
}

// GETTERS

float Line::getLinewidth() const
{
    return linewidth;
}

QPointF Line::getPos() const
{
    return p1();
}

// SETTERS

void Line::setLinewidth(float w)
{
    linewidth = w;
}

void Line::setPos(const QPointF &pos)
{
    setP1(pos);
}

// SET/GET PROPERTY
void Line::setProperty(const QString &name, const QString &value)
{
    float val;
    bool conversion_ok;

#define set_float_property(NAME, COMMAND) \
    if (name == NAME) { \
        val = value.toFloat(&conversion_ok); \
        if (!conversion_ok) \
            throw std::logic_error("Line property \"" + name.toStdString() + "\" is of invalid format"); \
        COMMAND; \
        return; \
    }

    set_float_property("x1", setP1({val, p1().y()}));
    set_float_property("y1", setP1({p1().x(), val}));
    set_float_property("x2", setP2({val, p2().y()}));
    set_float_property("y2", setP2({p2().x(), val}));

    set_float_property("linewidth", setLinewidth(val));
    set_float_property("width", setLinewidth(val));
    set_float_property("lw", setLinewidth(val));
    // else
    throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for line");
}
QString Line::getProperty(const QString &name) const
{
    if (name == "x1") return QString::number(p1().x());
    if (name == "y1") return QString::number(p1().y());
    if (name == "x2") return QString::number(p2().x());
    if (name == "y2") return QString::number(p2().y());
    if (name == "linewidth" || name == "lw" || name == "width") return QString::number(linewidth);
    // else
    throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for line");
}

// OPERATORS

bool Line::operator==(const Line &obj) const
{
    return Object::operator==(obj) && QLineF::operator==(obj) && linewidth == obj.linewidth;
}

bool Line::operator!=(const Line &obj) const
{
    return !(*this == obj);
}
