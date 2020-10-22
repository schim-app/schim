#include "sheetscene.h"

#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/commands.h"
#include "ui/operations.h"
#include "global.h"

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

    // Init cursor guides, set their display properties
    //initGuides(); //TODO remove completely

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

void SheetScene::initGuides()
{
    hGuide = addLine({});
    vGuide = addLine({});
    hGuide->setPos({});
    vGuide->setPos({});

    QPen pen(QColor{96, 96, 96}, 0);
    hGuide->setPen(pen);
    vGuide->setPen(pen);
}

QPointF SheetScene::snap(const QPointF &pt) const
{
    if (!snapEnabled)
        return pt;

    QPointF center = sheet->getContentArea().center();
    return center + QPointF{round((pt.x() - center.x()) / gridX) * gridX, round((pt.y() - center.y()) / gridY) * gridY};
}

void SheetScene::showGuides(bool show)
{
    showCursorGuides = show;
}

void SheetScene::updateGuides()
{
    return; //TODO remove the whole functiou
    QPointF pos = cursorPos;
    if (snapGuides)
        pos = snap(pos);

    // The scene rect
    QRectF rect = {0, 0, width(), height()};
    hGuide->setLine(rect.left(), pos.y(), rect.right(), pos.y());
    vGuide->setLine(pos.x(), rect.top(), pos.x(), rect.bottom());
}

/*****************
 * Miscellaneous *
******************/

void SheetScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QPen pen(Qt::black, dpiInvariant(1));
    pen.setCosmetic(true);
    painter->setPen(pen);

    // Draw the grid
    if (gridEnabled)
    {
        //TODO hide this implementation detail in a function in global.h
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

    // Draw the cursor guides
    if (showCursorGuides)
    {
        QPointF pos = getCursorPos();

        if (getSnapCursorGuides())
            pos = snap(pos);

        pen.setColor({64, 64, 64});
        painter->setPen(pen);

        // Horizontal
        painter->drawLine(QLineF{rect.left(), pos.y(), rect.right(), pos.y()});
        // Vertical
        painter->drawLine(QLineF{pos.x(), rect.top(), pos.x(), rect.bottom()});
    }
}

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
    else if (event->key() == Qt::Key_Delete) //TODO create an action
        command(new CmdDeleteSelection(selectedItems(), this));
}

void SheetScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    cursorPos = event->scenePos();

    if (operation)
        operation->mousePressEvent(event);
    else
        QGraphicsScene::mousePressEvent(event);
}

void SheetScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    cursorPos = event->scenePos();

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
