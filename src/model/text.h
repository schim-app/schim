#ifndef TEXT_H
#define TEXT_H

#include "object.h"

#include <QRectF>
#include <QString>

class Text : public Object, public QPointF
{
public:
    Text(const QString &text = "");

    void setPos(const QPointF &pos) override;
    void setText(const QString &text);
    void setTextHeight(float height);
    void setFont(const QString &name);

    QPointF getPos() const override;
    QString getText() const;
    float getTextHeight() const;
    QString getFont() const;

private:
    QString text, font;
    float textHeight = 5;

    friend class GText;
};

#endif // TEXT_H
