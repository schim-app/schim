#include "gcomponent.h"

#include "gterminal.h"
#include "gtext.h"

#include "ui/mainwindow.h"
#include "ui/windows/componentsettings.h"

#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

#include "model/text.h"
#include "ui/operations.h"

// CONSTRUCTORS

GComponent::GComponent(Component *obj)
    : GCompositeObject(obj)
{ }

// GETTERS

Component *GComponent::get()
{
    return (Component*) obj;
}

const Component *GComponent::get() const
{
    return (Component*) obj;
}

// USER ACTIONS

void GComponent::showContextMenu()
{
    QMenu contextMenu(MainWindow::getInstance());
    QAction edit("Edit element");
    QAction addText("Add text");
    contextMenu.addAction(&edit);
    contextMenu.addAction(&addText);
    // Connections
    QObject::connect(&edit, &QAction::triggered,
                     this, &GComponent::onContextEdit);
    QObject::connect(&addText, &QAction::triggered, this, [this]() {
        GText *obj = new GText; // Create new graphical text object
        TextInsertOperation *op = new TextInsertOperation(scene(), obj);
        obj->setParentItem(this); // The operation must know this text has a parent
        scene()->startOperation(op);
    });
    contextMenu.exec(QCursor::pos());
}

void GComponent::onContextEdit()
{
    ComponentSettings(this, MainWindow::getInstance()).exec();
}

// EVENTS

void GComponent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    GCompositeObject::mousePressEvent(event);

    if (event->buttons() == Qt::RightButton)
        showContextMenu();
}

void GComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    GCompositeObject::mouseDoubleClickEvent(event);
    if (event->buttons() == Qt::LeftButton)
        onContextEdit();
}

void GComponent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    for (auto *child : childItems())
    {
        if (dynamic_cast<GTerminal*>(child))
            child->show();
    }
    GCompositeObject::hoverEnterEvent(event);
}

void GComponent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    for (auto *child : childItems())
    {
        if (dynamic_cast<GTerminal*>(child))
            child->hide();
    }
    GCompositeObject::hoverLeaveEvent(event);
}

QVariant GComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{
    auto val = GCompositeObject::itemChange(change, value);
    if (change == ItemSceneHasChanged && scene() != nullptr)
    {
        // Add texts
        for (auto *text : get()->getTexts())
        {
            auto *assignee = new GText(text);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
        }
        // Add terminals
        for (auto *terminal : get()->getTerminals())
        {
            auto *assignee = new GTerminal(terminal);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
            // The terminal is hidden unless the component is hovered
            assignee->hide();
        }
    }
    else if (change == ItemSelectedHasChanged)
    {
    }
    return val;
}
