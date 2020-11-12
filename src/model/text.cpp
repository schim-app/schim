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

Object *Text::clone() const
{
    return new Text(*this);
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
