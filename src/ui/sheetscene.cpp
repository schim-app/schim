#include "sheetscene.h"

#include "mainwindow.h"
#include "objects/gobject.h"
#include "ui/objects/gline.h"
#include "ui/objects/gheader.h"
#include "ui/commands.h"
#include "ui/operations.h"
#include "global.h"
#include "ui/widgets/componentlist.h"

#include <QAction>
#include <QColor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>
#include <QMessageBox>

//TODO remove
#include <QDebug>
#include <iostream>
#include <QTreeView>
#include <QApplication>

SheetScene::SheetScene(Sheet *sheet)
    : sheet(sheet)
{
    if (sheet == nullptr) return;

    pageBackgroundItem = addRect({0, 0, sheet->getWidth(), sheet->getHeight()},
                                 {qApp->palette().color(QPalette::Text), 0}, qApp->palette().color(QPalette::Base));

    // Add the sheet header, if it is defined
    if (sheet->getHeader())
        setHeader(sheet->getHeader());

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

void SheetScene::setHeader(Header *hdr, bool destroy)
{
    if (headerItem != nullptr)
        removeItem(headerItem);
    delete headerItem;
    headerItem = (GHeader*) GObject::assign(hdr);
    addItem(headerItem);

    // Update model
    sheet->setHeader(hdr, destroy);
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

void SheetScene::cursorLeft(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos - QPointF{n * gridX, 0}))
                );
    emit cursorMoved();
}

void SheetScene::cursorDown(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos + QPointF{0, n * gridY}))
                );
    emit cursorMoved();
}

void SheetScene::cursorUp(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos - QPointF{0, n * gridY}))
                );
    emit cursorMoved();
}

void SheetScene::cursorRight(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(cursorPos + QPointF{n * gridX, 0}))
                );
    emit cursorMoved();
}

void SheetScene::gridIncrease(Vim::N n)
{
    setGridSize(getGridSize() + QSizeF{double(n), double(n)});
    update();
}

void SheetScene::gridDecrease(Vim::N n)
{
    setGridSize(getGridSize() - QSizeF{double(n), double(n)});
    update();
}

void SheetScene::startOperation(Operation *op)
{
    if (operation)
        operation->cancel();
    operation = op;
    connect(operation, &Operation::finished, this, &SheetScene::onOperationFinished);
}

void SheetScene::selectTexts()
{
    foreach (auto *obj, items())
        if (dynamic_cast<GText*>(obj))
            obj->setSelected(true);
}

void SheetScene::selectPrimitive()
{
    foreach (auto *obj, items())
        if (!dynamic_cast<GCompositeObject*>(obj))
            obj->setSelected(true);
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

bool SheetScene::askHeaderChangeConfirmation() const
{
    if (sheet->getHeader() == nullptr ||
            sheet->getHeader()->getSourceFile() == "")
        return QMessageBox::question(nullptr, "Confirmation", "The local header will be deleted. Proceed?")
                == QMessageBox::Yes;
    return true;
}

void SheetScene::tryChangeHeader(Header *hdr, bool *changed, bool *confirmed)
{
    if (changed) *changed = false;
    if (confirmed) *confirmed = true;
    bool hadLocalHdr = sheet->getHeader() && sheet->getHeader()->getSourceFile() == "";
    // ONLY one of them is null or they are both not null and they are not equal
    bool newHdrDifferent = ((!hdr && sheet->getHeader()) || (hdr && !sheet->getHeader()))
            || (hdr && sheet->getHeader() && *hdr != *sheet->getHeader());

    if (!newHdrDifferent) return;
    if (hadLocalHdr && !askHeaderChangeConfirmation())
    {
        if (confirmed)
        return;
    }

    // Upon confirmation, change the header
    command(new CmdChangeHeader(hdr, this));
    if (changed) *changed = true;
}

// HELPERS

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
    // TODO
    QGraphicsSceneMouseEvent mouseEvent;
    mouseEvent.setScenePos(pt);
    mouseMoveEvent(&mouseEvent);

    // Send hover event to item and each of its parents
    QGraphicsSceneHoverEvent hoverEvent;
    hoverEvent.setScenePos(pt);
    hoveredItem = nullptr;
    for (auto *item = itemAt(pt,{}); item != nullptr; item = item->parentItem())
    {
        if (item->acceptHoverEvents())
        {
            item->hoverEnterEvent(&hoverEvent);
            hoveredItem = item;
        }
    }

    // Update the cursor guides
    update();
}

void SheetScene::insertComponentOrHeader(Object *obj)
{
    Header *hdr;
    if ((hdr = dynamic_cast<Header*>(obj->clone())))
        // Replace the current sheet header with this one
        tryChangeHeader(hdr);
    else // Start scene operation
        startOperation(new ComponentInsertOperation(this, obj->clone()));
}

// OVERRIDDEN

GObject *SheetScene::itemAt(const QPointF &pt, const QTransform &deviceTransform)
{
    return static_cast<GObject*>(QGraphicsScene::itemAt(pt, deviceTransform));
}

void SheetScene::reload()
{
    for (auto *obj : items())
        if (dynamic_cast<GObject *>(obj))
            static_cast<GObject *>(obj)->reload();
}

// EVENTS

void SheetScene::keyPressEvent(QKeyEvent *event)
{
    // Let children process this event as needed, and potentially accept it
    QGraphicsScene::keyPressEvent(event);
    if (event->isAccepted()) return;

    Vim::registerKeyPress(event, [this](const Vim::Action &action) {
        return processVimAction(action);
    });

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
    { // A component is being dragged in
        auto *componentList = (ComponentList*) event->source();
        auto indexes = componentList->selectionModel()->selectedIndexes();
        if (indexes.size() > 0) // At least one component is selected
        {
            // Only the first selected component will be inserted
            auto *obj = static_cast<DatabaseItem*>(indexes[0].internalPointer())->getObject();
            insertComponentOrHeader(obj);
        }
    }
}

bool SheetScene::processVimAction(const Vim::Action &action)
{
#define if_eq_do(name, act) if (action == name) { act(Vim::n()); }

         if_eq_do("left",               	cursorLeft)
    else if_eq_do("down",					cursorDown)
    else if_eq_do("up",						cursorUp)
    else if_eq_do("right", 					cursorRight)
    else if_eq_do("grid-increase",			gridIncrease)
    else if_eq_do("grid-decrease", 			gridDecrease)
    else if (action == "select-texts")		selectTexts();
    else if (action == "select-primitive")	selectPrimitive();
    else return false;
    return true;
}

// SLOTS

void SheetScene::onOperationFinished()
{
    operation->deleteLater();
    operation = nullptr;
}
