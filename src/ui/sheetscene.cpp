#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/commands.h"

#include <QAction>
#include <QColor>
#include <QKeyEvent>

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

void SheetScene::insertLine()
{
    _insertingLine = true;
}

void SheetScene::undo()
{
    undoStack.undo();
}

void SheetScene::redo()
{
    undoStack.redo();
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
        clearSelection();
    else if (event->key() == Qt::Key_Delete) //TODO create an action
    {
        auto x = selectedItems();
        undoStack.push(new CmdDeleteSelection(selectedItems(), this));
    }

    QGraphicsScene::keyPressEvent(event);
}

void SheetScene::processInsertLine(QMouseEvent *event)
{
}
