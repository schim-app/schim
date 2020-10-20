#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/commands.h"
#include "ui/operations.h"

#include <QAction>
#include <QColor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    if (sheet == nullptr) return;

    pageBackgroundItem = addRect({0, 0, sheet->getWidth(), sheet->getHeight()}, {}, QColorConstants::White);

    // Add the sheet header, if it is defined
    if (sheet->getHeader())
        addItem(GObject::assign(sheet->getHeader()));

    // Populate the scene with the sheet contents
    for (auto *obj : *sheet)
        addItem(GObject::assign(obj));

}

SheetScene::SheetScene()
    : SheetScene(nullptr) { }

Sheet *SheetScene::getSheet()
{
    return sheet;
}

void SheetScene::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
}

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

void SheetScene::startOperation(Operation *op)
{
    operation = op;
}

void SheetScene::operationFinished(bool success)
{
    delete operation;
    operation = nullptr;
}

/*****************
 * Miscellaneous *
******************/

void SheetScene::updatePageBackground(float zoomLevel)
{
    auto pen = pageBackgroundItem->pen();
    pen.setWidth(1 / zoomLevel);
    pageBackgroundItem->setPen(pen);
}

void SheetScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        clearSelection();
        if (operation)
            operation->cancel();
    }
    else if (event->key() == Qt::Key_Delete) //TODO create an action
        undoStack.push(new CmdDeleteSelection(selectedItems(), this));

    QGraphicsScene::keyPressEvent(event);
}

void SheetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (operation)
        operation->mousePressEvent(event);
    else
        QGraphicsScene::mousePressEvent(event);
}

void SheetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (operation)
        operation->mouseMoveEvent(event);
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void SheetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (operation)
        operation->mouseReleaseEvent(event);
    else
        QGraphicsScene::mouseReleaseEvent(event);
}
