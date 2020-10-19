#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/commands.h"

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

void SheetScene::insertObject(GObject *obj)
{
    _insertingObject = obj;
    _insertingState = 0;
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
    {
        clearSelection();
        removeItem(_insertingObject);
        delete _insertingObject;
        _insertingObject = nullptr;
        _insertingState = 0;
    }
    else if (event->key() == Qt::Key_Delete) //TODO create an action
        undoStack.push(new CmdDeleteSelection(selectedItems(), this));

    QGraphicsScene::keyPressEvent(event);
}

void SheetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        auto pos = event->scenePos();
        if (dynamic_cast<GLine*>(_insertingObject))
        {
            //TODO one of these days I shall implement a class called Operation and
            // implement this functionality there
            auto *obj = (GLine*) _insertingObject;
            if (_insertingState == 0)
            {
                obj->get()->setLine(pos.x(), pos.y(), pos.x(), pos.y());
                obj->reload();
                addItem(obj);
                ++_insertingState;
            }
            else if (_insertingState == 1)
            {
                obj->get()->setP2(pos);
                obj->reload();

                sheet->append(obj->get());
                _insertingObject = nullptr;
                _insertingState = 0;
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void SheetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_insertingState == 1 && dynamic_cast<GLine*>(_insertingObject))
    {
        auto *obj = (GLine*) _insertingObject;
        obj->get()->setP2(event->scenePos());
        obj->reload();
        update();
    }
    else
        QGraphicsScene::mouseMoveEvent(event);
}

void SheetScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}
