#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/commands.h"
#include "ui/operations.h"

#include <QAction>
#include <QColor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

// Static variables in SheetScene
float SheetScene::gridX = 5, SheetScene::gridY = 5;
bool SheetScene::gridEnabled = 5, SheetScene::snapEnabled = 5;

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

Sheet *SheetScene::getSheet()
{
    return sheet;
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

void SheetScene::setSheet(Sheet *sheet)
{
    this->sheet = sheet;
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
    if (operation)
        operation->cancel();
    operation = op;
}

void SheetScene::operationFinished(bool success)
{
    delete operation;
    operation = nullptr;
}

QPointF SheetScene::snapToGrid(const QPointF &pt)
{
    if (!snapEnabled)
        return pt;

    QPointF center = sheet->getContentArea().center();
    return center + QPointF{round((pt.x() - center.x()) / gridX) * gridX, round((pt.y() - center.y()) / gridY) * gridY};
}

/*****************
 * Miscellaneous *
******************/

void SheetScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->setPen({Qt::black, 0});
    QRectF contentArea = sheet->getContentArea();
    QPointF center = contentArea.center();
    float Xmin = qMax(rect.left(), contentArea.left()),
            Xmax = qMin(rect.right(), contentArea.right()),
            Ymin = qMax(rect.top(), contentArea.top()),
            Ymax = qMin(rect.bottom(), contentArea.bottom());
    int Imax = (Xmax - center.x()) / gridX,
            Jmax = (Ymax - center.y()) / gridY;
    for (int i = qCeil((Xmin - center.x()) / gridX); i <= Imax; ++i)
        for (int j = qCeil((Ymin - center.y()) / gridY); j <= Jmax; ++j)
            painter->drawPoint(QPointF{center.x() + i * gridX, center.y() + j * gridY});
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
        command(new CmdDeleteSelection(selectedItems(), this));

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
