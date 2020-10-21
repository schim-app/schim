#include "gtext.h"

#include <QGraphicsSceneEvent>
#include <QTextCursor>

GText::GText(Text *obj)
    : GObject(obj), QGraphicsTextItem(obj->getText())
{
    reload();
}

QPainterPath GText::shape() const
{
    return QGraphicsTextItem::shape();
}

void GText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);
    QGraphicsTextItem::paint(painter, option, widget);

    return; //TODO remove the rest

    QFont font;
    if (get()->getFont() != "")
        font.setFamily(get()->getFont());
    font.setPixelSize(get()->getTextHeight());

    painter->setFont(font);
    painter->drawText(QPointF{0, get()->getTextHeight()}, get()->getText());
}

QRectF GText::boundingRect() const
{
    return QGraphicsTextItem::boundingRect();
    auto x = shape().boundingRect().marginsAdded({2, 2, 2, 2});
    return shape().boundingRect().marginsAdded({2, 2, 2, 2});
}

QVariant GText::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return GObject::itemChange(change, value);
}

void GText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::mousePressEvent(event);
        GObject::scene()->update();
    }
    else
        GObject::mousePressEvent(event);
}

void GText::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
        QGraphicsTextItem::mouseMoveEvent(event);
    else
        GObject::mouseMoveEvent(event);
}

void GText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::mouseReleaseEvent(event);
        GObject::scene()->update();
    }
    else
        GObject::mouseReleaseEvent(event);
}

void GText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && textInteractionFlags() == Qt::NoTextInteraction)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);
        GObject::setFlag(GObject::ItemIsFocusable);
        GObject::setFocus();

        GObject::setCursor(QCursor(Qt::IBeamCursor));
    }
    else
        GObject::mouseDoubleClickEvent(event);
}

void GText::keyPressEvent(QKeyEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::keyPressEvent(event);
        GObject::scene()->update();
    }
    else
        GObject::keyPressEvent(event);
}

void GText::keyReleaseEvent(QKeyEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::keyPressEvent(event);
        GObject::scene()->update();
    }
    else
        GObject::keyReleaseEvent(event);
}

void GText::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);

    GObject::unsetCursor();

    auto tc = textCursor();
    tc.clearSelection();
    setTextCursor(tc);

    QGraphicsTextItem::focusOutEvent(event);
}

void GText::reload()
{
    GObject::setPos(get()->getPos());
}

Text *GText::get()
{
    return (Text*) obj;
}

const Text *GText::get() const
{
    return (Text*) obj;
}
