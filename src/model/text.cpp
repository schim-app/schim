#include "text.h"

#include "model/compositeobject.h"
#include "model/sheet.h"

#include <QRegExp>
#include <QStringList>

Text::Text(const QString &text)
    : text(text) { }

Text::Text(const Text &text)
    : Object(), QPointF(text)
{
    this->text = text.text;
    font = text.font;
    textHeight = text.textHeight;
}

void Text::setPos(const QPointF &pos)
{
    setX(pos.x());
    setY(pos.y());
}

void Text::setText(const QString &text)
{
    this->text = text;
}

void Text::setTextHeight(float height)
{
    textHeight = height;
}

void Text::setFont(const QString &name)
{
    font = name;
}

QPointF Text::getPos() const
{
    return *this;
}

QString Text::getText() const
{
    return text;
}

QString Text::getDisplayText(const VariableSet &variableSet) const
{
    return Variable::substitute(getText(), variableSet);
}

QString Text::getDisplayText() const
{
    if (parent)
        return getDisplayText(parent->getVariables());
    else if (sheet)
        return getDisplayText(sheet->getVariables());
    else
        return getDisplayText({});
}

float Text::getTextHeight() const
{
    return textHeight;
}

QString Text::getFont() const
{
    return font;
}

void Text::setProperty(const QString &name, const QString &value)
{
    float val;
    bool conversion_ok;

#define set_float_property(NAME, COMMAND) \
    if (name == NAME) { \
        val = value.toFloat(&conversion_ok); \
        if (!conversion_ok) \
            throw std::logic_error("Text property \"" + name.toStdString() + "\" is of invalid format"); \
        COMMAND; \
        return; \
    }

    set_float_property("x", setPos({val, getPos().y()}));
    set_float_property("y", setPos({getPos().x(), val}));
    set_float_property("height", setTextHeight(val));
    if (name == "text") setText(value);
    else if (name == "font") setFont(value);
    else throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for text");
}

QString Text::getProperty(const QString &name) const
{
    if (name == "x") return QString::number(getPos().x());
    if (name == "y") return QString::number(getPos().y());
    if (name == "height") return QString::number(getTextHeight());
    if (name == "text") return getText();
    if (name == "font") return getFont();
    else throw std::logic_error("Unknown property \"" + name.toStdString() + "\" for text");
}

Object *Text::clone() const
{
    return new Text(*this);
}
