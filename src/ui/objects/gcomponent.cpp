#include "gcomponent.h"

#include "ui/mainwindow.h"
#include "ui/windows/componenteditor.h"

#include <QAction>
#include <QMenu>
#include <QGraphicsSceneMouseEvent>

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

// SLOTS

void GComponent::edit()
{
    ComponentEditor editor(this, MainWindow::getInstance());

    editor.exec();
}
