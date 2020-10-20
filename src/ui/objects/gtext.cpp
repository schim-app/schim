#include "gtext.h"

GText::GText(Text *obj)
    : GObject(obj)
{
    reload();
}

QPainterPath GText::shape() const
{
    QPainterPath path;
    path.addRect(QRectF{0, 0, getWidth(), get()->getTextHeight()}.marginsAdded({2, 2, 2, 2}));
    return path;
}

void GText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QFont font;
    if (get()->getFont() != "")
        font.setFamily(get()->getFont());
    font.setPixelSize(get()->getTextHeight());

    painter->setFont(font);
    painter->drawText(QPointF{0, get()->getTextHeight() / 2}, get()->getText());

    painter->setPen(QPen(Qt::black, 0, Qt::DashLine));
    painter->drawPath(shape());
}

QRectF GText::boundingRect() const
{
    auto x = shape().boundingRect().marginsAdded({2, 2, 2, 2});
    return shape().boundingRect().marginsAdded({2, 2, 2, 2});
}

void GText::reload()
{
    setPos(get()->getPos());
}

Text *GText::get()
{
    return (Text*) obj;
}

const Text *GText::get() const
{
    return (Text*) obj;
}

float GText::getWidth() const
{
    QFont font;
    if (get()->getFont() != "")
        font.setFamily(get()->getFont());
    font.setPixelSize(get()->getTextHeight());
    QFontMetrics fm(font);
    QString s = get()->getText();

    return fm.horizontalAdvance(get()->getText());
}
