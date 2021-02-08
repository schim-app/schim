#ifndef TEXT_H
#define TEXT_H

#include "object.h"
#include "variable.h"

#include <QRectF>
#include <QString>

class Text : public Object, public QPointF
{
public:
    Text(const QString &text = "");
    /** @brief Copy constructor */
    Text(const Text &text);
    Object *clone() const override;

    void setPos(const QPointF &pos) override;
    void setText(const QString &text);
    void setTextHeight(float height);
    void setFont(const QString &name);

    QPointF getPos() const override;
    QString getText() const;
    QString getDisplayText(const VariableSet &variableList) const;
    QString getDisplayText() const;
    float getTextHeight() const;
    QString getFont() const;

    void setProperty(const QString &name, const QString &value) override;
    QString getProperty(const QString &name) const override;

    // OPERATORS
    bool operator==(const Text &obj) const;
    bool operator!=(const Text &obj) const;

private:
    QString text, font = "Sans Serif";
    float textHeight = 4;

    friend class GText;
};

#endif // TEXT_H
