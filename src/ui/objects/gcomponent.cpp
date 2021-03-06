#include "gcomponent.h"
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

QVariant GComponent::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemSceneHasChanged && scene() != nullptr)
    {
        for (auto *child : *get())
        {
            auto *assignee = GObject::assign(child);
            scene()->addItem(assignee);
            assignee->setParentItem(this);
            if (!dynamic_cast<Text*>(child))
                assignee->setFlags({});
        }
        return value;
    }

    return GCompositeObject::itemChange(change, value);
}
