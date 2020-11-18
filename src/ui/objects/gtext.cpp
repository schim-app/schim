#include "gtext.h"

#include <QGraphicsSceneEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QTimer>

GText::GText(Text *obj)
    : GObject(obj), QGraphicsTextItem(obj->getDisplayText())
{
    reload();
    GObject::setAcceptHoverEvents(true);
}

// GETTERS

Text *GText::get()
{
    return (Text*) obj;
}

const Text *GText::get() const
{
    return (Text*) obj;
}

void GText::setEditable(bool editable)
{
    static int timerId;
    if (editable)
    {
        setPlainText(get()->getText());

        setTextInteractionFlags(Qt::TextEditorInteraction);
        GObject::setFlag(GObject::ItemIsFocusable);
        GObject::setFocus();

        GObject::setCursor(QCursor(Qt::IBeamCursor));
        timerId = GObject::startTimer(250);
    }
    else
    {
        setPlainText(get()->getDisplayText());
        setTextInteractionFlags(Qt::NoTextInteraction);

        GObject::unsetCursor();

        auto tc = textCursor();
        tc.clearSelection();
        setTextCursor(tc);

        GObject::killTimer(timerId);
    }
}

// OVERRIDEN QGraphicsItem METHODS

QPainterPath GText::shape() const
{
    return QGraphicsTextItem::shape();
}

void GText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    GObject::paint(painter, option, widget);
    setDefaultTextColor(Qt::black);
    QGraphicsTextItem::paint(painter, option, widget);
}

QRectF GText::boundingRect() const
{
    return QGraphicsTextItem::boundingRect();
}

SheetScene *GText::scene()
{
    return GObject::scene();
}

// EVENTS

QVariant GText::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return GObject::itemChange(change, value);
}

void GText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction) // We are in editing mode
    {
        // Update the cursor
        scene()->update();
        QGraphicsTextItem::mousePressEvent(event);
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
        // Override default behavior of the view which shows the guides when left button is released.
        scene()->showGuides(false);
    }
    else
        GObject::mouseReleaseEvent(event);
}

void GText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton && textInteractionFlags() == Qt::NoTextInteraction)
    {
        setEditable();
        GObject::scene()->showGuides(false);
    }
    else
        GObject::mouseDoubleClickEvent(event);
}

void GText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
        GObject::scene()->showGuides(false);
    QGraphicsTextItem::hoverEnterEvent(event);
}

void GText::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    GObject::scene()->showGuides(true);
    QGraphicsTextItem::hoverEnterEvent(event);
}

void GText::keyPressEvent(QKeyEvent *event)
{
    if (textInteractionFlags() == Qt::TextEditorInteraction)
    {
        QGraphicsTextItem::keyPressEvent(event);
        // Prevent pressing delete from deleting the object, and disable action keybindings
        event->accept();
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
        apply();
    }
    else
        GObject::keyReleaseEvent(event);
}

void GText::focusInEvent(QFocusEvent *event)
{
    keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_A, {}));
    keyPressEvent(new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, {}));
    QGraphicsTextItem::focusInEvent(event);
}

void GText::focusOutEvent(QFocusEvent *event)
{
    setEditable(false);
    QGraphicsTextItem::focusOutEvent(event);
}

void GText::timerEvent(QTimerEvent *event)
{
    GObject::update();
    QGraphicsTextItem::timerEvent(event);
}

// FOR EDITING THE OBJECT

void GText::reload()
{
    GObject::setPos(get()->getPos());
    QFont font;
    if (get()->getFont() != "")
        font.setFamily(get()->getFont());
    font.setPixelSize(get()->getTextHeight());
    setFont(font);
}

void GText::apply()
{
    get()->setPos(GObject::pos());
    get()->setText(toPlainText());
}
