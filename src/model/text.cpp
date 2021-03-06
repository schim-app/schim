#include "text.h"

#include "model/compositeobject.h"
#include "model/sheet.h"

// CONSTRUCTORS

Text::Text(const QString &text)
    : text(text) { }

Text::Text(const Text &text)
    : Object(), text(text.text), font(text.font),
      textHeight(text.textHeight) { }

Object *Text::clone() const
{
    return new Text(*this);
}

// GETTERS

QPointF Text::getPos() const
{
    return pos;
}

QString Text::getText() const
{
    return text;
}

float Text::getTextHeight() const
{
    return textHeight;
}

QString Text::getDisplayText(const VariableSet &variableSet) const
{
    return Variable::substitute(getText(), variableSet);
}

QString Text::getDisplayText(const Entity *context) const
{
    if (context)
        return getDisplayText(context->getVariables());
    else
        return getDisplayText(VariableSet{});
}

QString Text::getFont() const
{
    return font;
}

// SETTERS

void Text::setPos(const QPointF &pos)
{
    this->pos = pos;
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

// SET/GET PROPERTY
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

// OPERATORS

bool Text::operator==(const Text &obj) const
{
    return Object::operator==(obj) && getPos() == obj.getPos() &&
            textHeight == obj.textHeight && font == obj.font;
}

bool Text::operator!=(const Text &obj) const
{
    return !(*this == obj);
}
