#include "gcomponent.h"

#include "model/text.h"

#include "gterminal.h"
#include "gtext.h"

#include "ui/mainwindow.h"
#include "ui/operations.h"
#include "ui/windows/componentsettings.h"

#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

#include <iostream>

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

// SLOTS

void GComponent::onContextEdit()
{
    ComponentSettings(this, MainWindow::getInstance()).exec();
}

// EVENTS

void GComponent::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    GCompositeObject::mouseDoubleClickEvent(event);
    if (event->buttons() == Qt::LeftButton)
        onContextEdit();
}

void GComponent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // TODO The hover event is not a 100% reliable way to determine if the mouse
    // is over the object. And mouseMoveEvent doesn't fire even when mouse
    // tracking is enabled in the view.
    for (auto *child : childItems())
        if (dynamic_cast<GTerminal*>(child))
            child->show();
    GCompositeObject::hoverEnterEvent(event);
}

void GComponent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    for (auto *child : childItems())
        if (dynamic_cast<GTerminal*>(child))
            child->hide();
    GCompositeObject::hoverLeaveEvent(event);
}

QVariant GComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{
    auto val = GCompositeObject::itemChange(change, value);
    if (change == ItemSceneHasChanged && getSheetScene() != nullptr)
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
    return val;
}

void GComponent::contextMenuEvent(QGraphicsSceneContextMenuEvent *)
{
    QMenu menu(MainWindow::getInstance());
    QAction edit("Edit element");
    QAction addText("Add text");
    menu.addAction(&edit);
    menu.addAction(&addText);
    // Connections
    QObject::connect(&edit, &QAction::triggered,
                     this, &GComponent::onContextEdit);
    QObject::connect(&addText, &QAction::triggered, this, [this]() {
        GText *obj = new GText; // Create new graphical text object
        OpInsertText *op = new OpInsertText(getSheetScene(), obj);
        obj->setParentItem(this); // The operation must know this text has a parent
        getSheetScene()->startOperation(op);
    });
    menu.exec(QCursor::pos());
}
