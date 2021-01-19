#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/commands.h"
#include "ui/operations.h"
#include "global.h"
#include "ui/widgets/componentlist.h"

#include <QAction>
#include <QColor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

//TODO remove
#include <QDebug>
#include <iostream>
#include <QTreeView>
#include <QApplication>

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    if (sheet == nullptr) return;

    pageBackgroundItem = addRect({0, 0, sheet->getWidth(), sheet->getHeight()}, {Qt::black, 0}, Qt::white);

    // Add the sheet header, if it is defined
    if (sheet->getHeader())
        addItem(GObject::assign(sheet->getHeader()));

    // Populate the scene with the sheet contents
    for (auto *obj : *sheet)
        addItem(GObject::assign(obj));
}

SheetScene::SheetScene()
    : SheetScene(nullptr) { }

// GETTERS

Sheet *SheetScene::getSheet()
{
    return sheet;
}

const Sheet *SheetScene::getSheet() const
{
    return sheet;
}

QPointF SheetScene::getCursorPos() const
{
    return cursorPos;
}

QPointF SheetScene::getSnappedCursorPos() const
{
    return snap(getCursorPos());
}

bool SheetScene::getSnapCursorGuides() const
{
    return snapGuides;
}

bool SheetScene::isGridEnabled()
{
    return gridEnabled;
}

bool SheetScene::isSnapEnabled()
{
    return snapEnabled;
}

QSizeF SheetScene::getGridSize()
{
    return {gridX, gridY};
}

// SETTERS

void SheetScene::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}

void SheetScene::setSnapCursorGuides(bool snap)
{
    snapGuides = snap;
}

void SheetScene::setGridSize(float x, float y)
{
    gridX = qMax(x, 1.f);
    gridY = qMax((y == -1 ? x : y), 1.f);
}

void SheetScene::setGridSize(QSizeF size)
{
    setGridSize(size.width(), size.height());
}

void SheetScene::setGridEnabled(bool enabled)
{
    gridEnabled = enabled;
}

void SheetScene::setSnapToGrid(bool snap)
{
    snapEnabled = snap;
}

// USER ACTIONS

void SheetScene::undo()
{
    undoStack.undo();
}

void SheetScene::redo()
{
    undoStack.redo();
}

void SheetScene::command(QUndoCommand *command)
{
    undoStack.push(command);
}

void SheetScene::cursorLeft()
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos - QPointF{gridX, 0}))
                );
}

void SheetScene::cursorDown()
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos + QPointF{0, gridY}))
                );
}

void SheetScene::cursorUp()
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos - QPointF{0, gridY}))
                );
}

void SheetScene::cursorRight()
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos + QPointF{gridX, 0}))
                );
}

void SheetScene::startOperation(Operation *op)
{
    if (operation)
        operation->cancel();
    operation = op;
    connect(operation, &Operation::finished, this, &SheetScene::onOperationFinished);
}

QPointF SheetScene::snap(const QPointF &pt) const
{
    if (!snapEnabled || sheet == nullptr)
        return pt;
    else
        return forcedSnap(pt);
}

QPointF SheetScene::forcedSnap(const QPointF &pt) const
{
    QPointF center = sheet->getContentArea().center();
    return center + QPointF{round((pt.x() - center.x()) / gridX) * gridX, round((pt.y() - center.y()) / gridY) * gridY};
}

void SheetScene::showGuides(bool show)
{
    showCursorGuides = show;
}

// HELPER FUNCTIONS

QPointF SheetScene::constrainToContentArea(QPointF pt) const
{
    //TODO maybe move this function to Sheet
    auto content = getSheet()->getContentArea();
    if (pt.x() > content.right())
        pt.setX(content.right());
    else if (pt.x() < content.left())
        pt.setX(content.left());

    if (pt.y() > content.bottom())
        pt.setY(content.bottom());
    else if (pt.y() < content.top())
        pt.setY(content.top());

    return pt;
}

void SheetScene::applyCursorMovement(const QPointF &pt)
{
    // Send mouse move event to item
    QGraphicsSceneMouseEvent mouseEvent;
    mouseEvent.setScenePos(pt);
    mouseMoveEvent(&mouseEvent);

    // Send hover event to item and each of its parents
    QGraphicsSceneHoverEvent hoverEvent;
    hoverEvent.setScenePos(pt);
    for (auto *item = itemAt(pt,{}); item != nullptr; item = item->parentItem())
        item->hoverEnterEvent(&hoverEvent);

    // Update the cursor guides
    update();
}

// OVERRIDEN METHODS

GObject *SheetScene::itemAt(const QPointF &pt, const QTransform &deviceTransform)
{
    return static_cast<GObject*>(QGraphicsScene::itemAt(pt, deviceTransform));
}

// EVENTS

void SheetScene::keyPressEvent(QKeyEvent *event)
{
    // Should be first to allow children to accept the event
    QGraphicsScene::keyPressEvent(event);

    if (event->isAccepted()) return;

    if (event->key() == Qt::Key_Escape)
    {
        clearSelection();
        if (operation)
            operation->cancel();
    }
    else if (event->key() == Qt::Key_Delete && !selectedItems().isEmpty()) //TODO create an action
        command(new CmdDeleteSelection(selectedItems(), this));
}

void SheetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    cursorPos = event->scenePos();

    if (operation)
    {
        clearSelection();
        operation->mousePressEvent(event);
    }
    else if (event->buttons() != Qt::MidButton)
        QGraphicsScene::mousePressEvent(event);
}

void SheetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    cursorPos = event->scenePos();

    QGraphicsSceneHoverEvent hoverEvent;
    hoverEvent.setScenePos(cursorPos);
    if (hoveredItem != nullptr)
    {
        auto *item = itemAt(cursorPos, {});
        // The item can be null when the event is triggered by zooming
        if (item != nullptr && hoveredItem != item->getOldestParent())
        {
            hoveredItem->hoverLeaveEvent(&hoverEvent);
            hoveredItem = nullptr;
        }
    }

    if (operation)
        operation->mouseMoveEvent(event);
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void SheetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    cursorPos = event->scenePos();
    if (operation)
        operation->mouseReleaseEvent(event);
    else
        QGraphicsScene::mouseReleaseEvent(event);
}

void SheetScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (dynamic_cast<ComponentList*>(event->source()))
        event->accept();
}

void SheetScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (dynamic_cast<ComponentList*>(event->source()))
        event->acceptProposedAction();
}

void SheetScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    cursorPos = event->scenePos();
    if (dynamic_cast<ComponentList*>(event->source()))
    {
        auto *componentList = (ComponentList*) event->source();
        auto indexes = componentList->selectionModel()->selectedIndexes();
        if (indexes.size() > 0)
        {
            auto *item = static_cast<DatabaseItem*>(indexes[0].internalPointer());
            startOperation(new ComponentInsertOperation(this, item->getObject()->clone()));
        }
    }
}

// SLOTS

void SheetScene::onOperationFinished()
{
    operation->deleteLater();
    operation = nullptr;
}
