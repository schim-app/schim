#include "gtext.h"

#include "ui/windows/textsettings.h"
#include "ui/mainwindow.h"
#include "global.h"

#include <QGraphicsSceneEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <QFont>
#include <QMenu>
#include <QTextDocument>

GText::GText(Text *obj)
    : GObject(obj)
{
    GObject::setAcceptHoverEvents(true);
    displayItem = new GDisplayText;
    displayItem->setDefaultTextColor(Qt::black);
    displayItem->setDocument(new QTextDocument);
    displayItem->document()->setDocumentMargin(1);
    connect(displayItem, SIGNAL(focusOut()), this, SLOT(onFocusOut()));
}

GText::~GText()
{
    delete displayItem->document();
    delete displayItem;
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

SheetScene *GText::scene()
{
    return GObject::scene();
}

// OVERRIDEN QGraphicsItem METHODS

QPainterPath GText::shape() const
{
    return displayItem->shape();
}

QRectF GText::boundingRect() const
{
    return displayItem->boundingRect();
}

void GText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::black, dpiInvariant(1), Qt::DashLine);

    if (isSelected() || isHovered() || editMode)
    {
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->drawPath(shape());
    }
}

// EVENTS

QVariant GText::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSceneHasChanged && scene() != nullptr)
    {
        scene()->addItem(displayItem);
        displayItem->setParentItem(this);
        displayItem->setZValue(zValue() - 0.1);
        displayItem->setFlags({});
    }
    return GObject::itemChange(change, value);
}

void GText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton)
        showContextMenu();
    else
        GObject::mousePressEvent(event);
}

void GText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    GObject::mouseReleaseEvent(event);
}

void GText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (!editMode && event->buttons() == Qt::LeftButton)
    {
        scene()->showGuides(false);
        setEditMode(true);
    }
    GObject::mouseDoubleClickEvent(event);
}

void GText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (editMode)
        scene()->showGuides(false);
    GObject::hoverEnterEvent(event);
}

void GText::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (editMode)
        scene()->showGuides(true);
    GObject::hoverLeaveEvent(event);
}

void GText::timerEvent(QTimerEvent *event)
{
    update();
    GObject::timerEvent(event);
}

// FOR EDITING THE OBJECT

void GText::reload()
{
    QFont font;
    if (get()->getFont() != "")
        font.fromString(get()->getFont());
    else
        font = QApplication::font();

    if (get()->getTextHeight() != -1)
        font.setPixelSize(get()->getTextHeight());

    displayItem->setFont(font);
    displayItem->setPlainText(get()->getDisplayText());
    setPos(get()->getPos());
}

void GText::apply()
{
    get()->setPos(GObject::pos());
    if (editMode)
        get()->setText(displayItem->toPlainText());
}

void GText::showContextMenu()
{
    QMenu contextMenu(MainWindow::getInstance());
    QAction edit("Edit text");
    QObject::connect(&edit, &QAction::triggered, this, &GText::edit);
    contextMenu.addAction(&edit);
    contextMenu.exec(QCursor::pos());
}

void GText::setEditMode(bool edit)
{
    static int timerId = -1;
    editMode = displayItem->editMode = edit;
    if (edit)
    {
        displayItem->setPlainText(get()->getText());

        displayItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        displayItem->setFlag(ItemIsFocusable);
        displayItem->setFocus();

        displayItem->setCursor(QCursor(Qt::IBeamCursor));
        timerId = startTimer(250);
    }
    else
    {
        displayItem->setPlainText(get()->getDisplayText());

        displayItem->setTextInteractionFlags(Qt::NoTextInteraction);
        displayItem->unsetCursor();

        auto tc = displayItem->textCursor();
        tc.clearSelection();
        displayItem->setTextCursor(tc);

        if (timerId != -1)
        {
            killTimer(timerId);
            timerId = -1;
        }
    }
}

// SLOTS

void GText::edit()
{
    TextSettings editor(this, MainWindow::getInstance());
    editor.exec();
}

void GText::onFocusOut()
{
    scene()->showGuides(true);
    apply(); // Has to be before setEditMode
    setEditMode(false);
    emit focusOut();
}

/****************
 * GDisplayText *
 ****************/

// EVENTS

void GDisplayText::focusOutEvent(QFocusEvent *event)
{
    editMode = false;
    emit focusOut();
    QGraphicsTextItem::focusOutEvent(event);
}

void GDisplayText::keyPressEvent(QKeyEvent *event)
{
    // Enter finalizes the edit, while SHIFT+Enter inserts a new line
    if (editMode && event->key() == Qt::Key_Return)
    {
        if (event->modifiers() == 0)
        {
            parentItem()->apply();
            parentItem()->setEditMode(false);
            return;
        }
        else if (event->modifiers() == Qt::SHIFT)
        {
            event->setModifiers({});
            QGraphicsTextItem::keyPressEvent(event);
            return;
        }
    }
    QGraphicsTextItem::keyPressEvent(event);
}

void GDisplayText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tweak(*option);
    // Remove the border so we can draw a custom one in GText
    tweak.state &= ~QStyle::State_HasFocus;
    QGraphicsTextItem::paint(painter, &tweak, widget);
}

// CUSTOM METHODS

SheetScene *GDisplayText::scene()
{
    return static_cast<SheetScene*>(QGraphicsTextItem::scene());
}

void GDisplayText::setParentItem(GText *parent)
{
    QGraphicsTextItem::setParentItem(parent);
}

GText *GDisplayText::parentItem()
{
    return (GText *) QGraphicsTextItem::parentItem();
}
