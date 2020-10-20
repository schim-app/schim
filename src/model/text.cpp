#include "text.h"

Text::Text(const QString &text)
    : text(text) { }

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

float Text::getTextHeight() const
{
    return textHeight;
}

QString Text::getFont() const
{
    return font;
}
