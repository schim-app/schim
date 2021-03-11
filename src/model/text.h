#ifndef TEXT_H
#define TEXT_H

#include "model/object.h"
#include "model/entity.h"
#include "model/variable.h"

#include <QRectF>
#include <QString>

class Text : public Object, public QPointF
{
public:
    // CONSTRUCTORS
    /// @brief Usual way to construct a text object.
    Text(const QString &text = "");
    /// @brief Copy constructor
    Text(const Text &text);
    Object *clone() const override;

    // GETTERS
    QPointF getPos() const override;
    QString getText() const;
    QString getDisplayText(const VariableSet &variableList) const;
    QString getDisplayText(const Entity *context) const;
    float getTextHeight() const;
    QString getFont() const;
    QString getProperty(const QString &name) const override;

    // SETTERS
    void setPos(const QPointF &pos) override;
    void setText(const QString &text);
    void setTextHeight(float height);
    void setFont(const QString &name);
    void setProperty(const QString &name, const QString &value) override;

    // OPERATORS
    bool operator==(const Text &obj) const;
    bool operator!=(const Text &obj) const;

private:
    QString text, font = "Sans Serif";
    float textHeight = 4;
};

#endif // TEXT_H
