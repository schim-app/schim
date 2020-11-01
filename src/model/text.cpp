#include "text.h"

#include <QRegExp>

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

QString Text::getDisplayText(const QList<Variable> &variableList) const
{ //TODO test this
    QString retVal, varName;
    QRegExp allowedChars("[a-zA-Z_]");
    bool percent = false;
    // Iterate through all characters of the text
    for (int i = 0; i < text.size(); ++i)
    {
        if (text[i] == '%')
        { // Whatever comes in the next iteration is a variable or another % sign
            if (percent)
                retVal += '%';
            // Reset the flag
            percent = !percent;
        }
        else if (percent)
        { // Previous char was %, ergo we will parse a variable
            if (allowedChars.exactMatch(text[i]))
                varName += text[i];
            else
            {
                //TODO find the variable from the list
                //retVal += variableList...
                --i;
                percent = false;
            }
        }
        else
            retVal += text[i];
    }
    return retVal;
}

float Text::getTextHeight() const
{
    return textHeight;
}

QString Text::getFont() const
{
    return font;
}
