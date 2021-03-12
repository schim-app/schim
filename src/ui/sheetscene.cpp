#include "sheetscene.h"

#include "global.h"
#include "mainwindow.h"
#include "objects/gobject.h"
#include "objects/gcomponent.h"
#include "ui/objects/gline.h"
#include "ui/objects/gheader.h"
#include "ui/objects/gterminal.h"
#include "ui/commands.h"
#include "ui/operations.h"
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
    if (sheet == nullptr)
        throw std::logic_error("Cannot construct a scene with a null sheet.");
    // Paper background item
    pageBackgroundItem = addRect({0, 0, sheet->getWidth(), sheet->getHeight()},
                     {qApp->palette().color(QPalette::Text), 0},
                     qApp->palette().color(QPalette::Base));

    // Add the sheet header, if it is defined
    if (sheet->getHeader())
        setHeader(sheet->getHeader());

    // Populate the scene with the sheet contents
    for (auto *obj : *sheet)
        addItem(GObject::assign(obj));

    connect(this, &SheetScene::selectionChanged,
            this, &SheetScene::onSelectionChanged);
}

// GETTERS

Sheet *SheetScene::getSheet()
{
    return sheet;
}

const Sheet *SheetScene::getSheet() const
{
    return sheet;
}

QPointF SheetScene::getRawCursorPos() const
{
    return rawCursorPos;
}

QPointF SheetScene::getCursorPos() const
{
    return rawCursorPos;
}

QPointF SheetScene::getSnappedCursorPos() const
{
    return snap(getCursorPos());
}

bool SheetScene::getSnapCursorGuides() const
{
    return snapGuides;
}

bool SheetScene::isGridEnabled() const
{
    return gridEnabled;
}

bool SheetScene::isSnapEnabled() const
{
    return snapEnabled;
}

QSizeF SheetScene::getGridSize() const
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

void SheetScene::setSnapEnabled(bool snap)
{
    snapEnabled = snap;
}

// USER ACTIONS

void SheetScene::undo(Vim::N n)
{
    n = qMin(int(n), 10);
    undoStack.undo();
}

void SheetScene::redo(Vim::N n)
{
    n = qMin(int(n), 10);
    undoStack.redo();
}

void SheetScene::command(QUndoCommand *command)
{
    undoStack.push(command);
}

void SheetScene::cursorLeft(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(rawCursorPos - QPointF{n * gridX, 0}))
                );
}

void SheetScene::cursorDown(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(rawCursorPos + QPointF{0, n * gridY}))
                );
}

void SheetScene::cursorUp(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(rawCursorPos - QPointF{0, n * gridY}))
                );
}

void SheetScene::cursorRight(Vim::N n)
{
    applyCursorMovement(
                constrainToContentArea(forcedSnap(rawCursorPos + QPointF{n * gridX, 0}))
                );
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

void SheetScene::startOperation(SceneOperation *op)
{
    if (operation) // Cancel any previous operation
        operation->cancel();
    operation = op; // Store the pointer
    connect(operation, &SceneOperation::finished,
            this, &SheetScene::onOperationFinished);
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

void SheetScene::insertLine()
{
    startOperation(new OpInsertLine(this));
}

void SheetScene::insertRect()
{
    startOperation(new OpInsertRect(this));
}

void SheetScene::insertText()
{
    startOperation(new OpInsertText(this));
}

void SheetScene::suggestConnections(GComponent *component)
{
    auto suggestions = getConnectionSuggestions(
                static_cast<GComponent*>(component));
    for (auto s : suggestions)
        registerConnectionSuggestion(s.first, s.second);
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

void SheetScene::tryChangeHeader(Header *hdr, bool *changed, bool *confirmed)
{
    if (changed) *changed = false;
    if (confirmed) *confirmed = true;
    bool hadLocalHdr = sheet->getHeader() && sheet->getHeader()->getFileName() == "";
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
    Header *hdr = dynamic_cast<Header*>(obj->clone());
    if (hdr)
        // Replace the current sheet header with this one
        tryChangeHeader(hdr);
    else // Start scene operation
        startOperation(new OpInsertComponent(this, obj->clone()));
}

bool SheetScene::askHeaderChangeConfirmation() const
{
    if (sheet->getHeader() == nullptr ||
            sheet->getHeader()->getFileName() == "")
        return QMessageBox::question(nullptr, "Confirmation", "The local header will be deleted. Proceed?")
                == QMessageBox::Yes;
    return true;
}

// Local helpers
float wrap_angle(float angleDeg)
{
    return angleDeg - int(angleDeg / 360) * 360;
}
bool are_compatible(Terminal::Prong p1, Terminal::Prong p2)
{
    float angle1 = wrap_angle(p1.getAngle()), angle2 = wrap_angle(p2.getAngle());
    float angDiff = p1.getAngle() - p2.getAngle();
    float n_360 = qAbs(angDiff - 180) / 360;
    QLineF line(p1.getTerminal()->getSheetPos(), p2.getTerminal()->getSheetPos());
    float angLine = line.angle();
    return qAbs(qAbs(angDiff) - 180) < 1e-4 && qAbs(angLine - angle1) < 1e-3;
}

QList<QPair<Terminal::Prong, Terminal::Prong>>
    SheetScene::getConnectionSuggestions(GComponent *component)
{
    Component *c = component->get();
    QList<QPair<Terminal::Prong, Terminal::Prong>> list;
    // Iterate through all other terminals
    for (auto *obj : getSheet()->getObjects())
    {
        // We only want components - skip unwanted objects
        if (!dynamic_cast<Component*>(obj)) continue;
        if (obj == c) continue;
        auto *c2 = static_cast<Component*>(obj);
        // The following loop can be viewed as a single loop through a nested structure
        // Iterate through ALL prongs in t1 and t2
        for (auto *t1 : c->getTerminals())
            for (auto *t2 : c2->getTerminals())
                for (auto p1 : t1->getProngs())
                    for (auto p2 : t2->getProngs())
                        // We are looping over p1 and p2
                        if (are_compatible(p1, p2))
                            list.append({p1, p2});
    }
    return list;
}

void SheetScene::registerConnectionSuggestion(Terminal::Prong a, Terminal::Prong b)
{
    auto *suggester = new GConnectionSuggester(a, b);
    addItem(suggester);
    _suggesters.append(suggester);
}

// OVERRIDDEN

GObject *SheetScene::itemAt(const QPointF &pt, const QTransform &deviceTransform)
{
    return dynamic_cast<GObject*>(QGraphicsScene::itemAt(pt, deviceTransform));
}

void SheetScene::reload()
{
    for (auto *obj : items())
        if (dynamic_cast<GObject *>(obj))
            static_cast<GObject *>(obj)->reloadFromModel();
}

// SLOTS

void SheetScene::onSelectionChanged()
{
    // Clear any previous connection suggestions
    for (auto *s : _suggesters)
    {
        removeItem(s);
        delete s;
    }
    _suggesters.clear();

    // Add potential connection suggestions
    auto selected = selectedItems();
    if (selected.size() == 1 && dynamic_cast<GComponent*>(selected[0]))
        suggestConnections((GComponent*) selected[0]);
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
    rawCursorPos = event->scenePos();

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
    rawCursorPos = event->scenePos();

    QGraphicsSceneHoverEvent hoverEvent;
    hoverEvent.setScenePos(rawCursorPos);
    if (hoveredItem != nullptr)
    {
        auto *item = itemAt(rawCursorPos, {});
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
    rawCursorPos = event->scenePos();
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
    rawCursorPos = event->scenePos();
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
    else if_eq_do("undo",					undo)
    else if_eq_do("redo",					redo)
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
