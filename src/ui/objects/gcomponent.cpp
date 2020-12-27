#include "gcomponent.h"

#include "ui/mainwindow.h"
#include "ui/windows/componentsettings.h"

#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

#include "model/text.h"

GComponent::GComponent(Component *obj)
    : GCompositeObject(obj)
{ }

Component *GComponent::get()
{
    return (Component*) obj;
}

const Component *GComponent::get() const
{
    return (Component*) obj;
}

void GComponent::showContextMenu()
{
    QMenu contextMenu(MainWindow::getInstance());
    QAction edit("Edit element");
    QObject::connect(&edit, &QAction::triggered, this, &GComponent::edit);
    contextMenu.addAction(&edit);
    contextMenu.exec(QCursor::pos());
}

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
        edit();
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

// SLOTS

void GComponent::edit()
{
    ComponentSettings editor(this, MainWindow::getInstance());

    editor.exec();
}
