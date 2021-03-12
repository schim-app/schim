#include "gtext.h"

#include "ui/windows/textsettings.h"
#include "ui/mainwindow.h"
#include "global.h"

#include "ui/objects/gcompositeobject.h"

#include <QGraphicsSceneEvent>
#include <QTextCursor>
#include <QKeyEvent>
#include <QTimer>
#include <QApplication>
#include <QFont>
#include <QMenu>
#include <QTextDocument>

#include <iostream>

// CONSTRUCTORS

GText::GText(Text *obj)
    : GObject(obj)
{
    GObject::setAcceptHoverEvents(true);
    displayItem = new GDisplayText;
    displayItem->setDefaultTextColor(qApp->palette().color(QPalette::Text));
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

bool GText::isInEditMode() const
{
    return displayItem->textInteractionFlags() & Qt::TextEditorInteraction;
}

// OBJECT EDITING

void GText::reloadFromModel()
{
    QFont font;
    if (get()->getFont() != "")
        font.fromString(get()->getFont());
    else
        font = QApplication::font();

    if (get()->getTextHeight() != -1)
        font.setPixelSize(get()->getTextHeight());

    displayItem->setFont(font);
    displayItem->setPlainText(get()->getDisplayText(getModelParent()));
    setPos(get()->getPos());
}

void GText::applyToModel()
{
    get()->setPos(GObject::pos());
    if (isInEditMode())
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
        displayItem->setPlainText(get()->getDisplayText(getModelParent()));

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

// EVENTS

QVariant GText::itemChange(QGraphicsItem::GraphicsItemChange change,
                           const QVariant &value)
{
    if (change == ItemSceneHasChanged && scene() != nullptr)
    { // Scene has changed: move displayItem along with me
        // TODO is this even necessary?
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
    if (!isInEditMode() && event->buttons() == Qt::LeftButton)
    {
        getSheetScene()->showGuides(false);
        setEditMode(true);
    }
    GObject::mouseDoubleClickEvent(event);
}

void GText::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (isInEditMode())
        getSheetScene()->showGuides(false);
    GObject::hoverEnterEvent(event);
}

void GText::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (isInEditMode())
        getSheetScene()->showGuides(true);
    GObject::hoverLeaveEvent(event);
}

void GText::timerEvent(QTimerEvent *event)
{
    update();
    GObject::timerEvent(event);
}

// SLOTS

void GText::edit()
{
    TextSettings editor(this, MainWindow::getInstance());
    editor.exec();
}

void GText::onFocusOut()
{
    getSheetScene()->showGuides(true);
    applyToModel(); // Has to be before setEditMode
    setEditMode(false);
    emit focusOut();
}

// OVERRIDE QGraphicsItem

QPainterPath GText::shape() const
{
    return displayItem->shape();
}

QRectF GText::boundingRect() const
{
    return displayItem->boundingRect();
}

void GText::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    // The text is drawn enclosed in a box if it is selectable and:
    // - It or its parent is selected, or
    // - It or its parent is hovered, or
    // - It is in edit mode
    if ((flags() & GraphicsItemFlag::ItemIsSelectable) &&
            (isSelected() || isHovered() || isInEditMode() ||
             (parentItem() && (parentItem()->isSelected() || parentItem()->isHovered()))))
    { // Draw the "selected" border around the text
        QPen pen(qApp->palette().color(QPalette::Text),
             dpiInvariant(1), Qt::DashLine);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->drawPath(shape());
    }
}

// class GDisplayText ***********************************************************

// EVENTS

void GDisplayText::focusOutEvent(QFocusEvent *event)
{
    emit focusOut();
    QGraphicsTextItem::focusOutEvent(event);
}

void GDisplayText::keyPressEvent(QKeyEvent *event)
{
    // Enter finalizes the edit, while SHIFT+Enter inserts a new line
    if (parentItem()->isInEditMode() && event->key() == Qt::Key_Return)
    {
        if (event->modifiers() == 0)
        {
            parentItem()->applyToModel();
            parentItem()->setEditMode(false);
            return;
        }
        else if (event->modifiers() == Qt::SHIFT)
            event->setModifiers({});
    }
    QGraphicsTextItem::keyPressEvent(event);
}

void GDisplayText::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tweak(*option);
    // Remove the border so we can draw a custom one in GText
    tweak.state &= ~QStyle::State_HasFocus;
    auto *higherParent = parentItem()->parentItem();

    setDefaultTextColor(qApp->palette().color(QPalette::Text));
    if (!higherParent || parentItem()->isInEditMode())
        return QGraphicsTextItem::paint(painter, &tweak, widget);
    else if (parentItem()->isSelected() || higherParent->isSelected())
        setDefaultTextColor(qApp->palette().color(QPalette::Highlight));
    else if (higherParent && higherParent->isHovered())
        // TODO the color source may be a little stupid, but it looks reasonably nice
        setDefaultTextColor(qApp->palette().color(QPalette::LinkVisited));

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
